/**
 * @file data_transmission.cpp
 * @brief IoT data transmission implementation
 * @author caTr1xLiu
 */

#include "data_transmission.hpp"
#include "wifi_info.hpp"
#include "game.hpp"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <queue>
#include <mutex>
#include <string>
#include <optional>

WiFiClientSecure wifi;
int status = WL_IDLE_STATUS;
HTTPClient http;

// Buffer to store up to 20 game sessions as JSON strings
constexpr int MAX_GAMES_IN_BUFFER = 20;
std::mutex bufferMutex;
std::queue<std::string> gameDataBuffer;

bool connectToWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(MYSSID, PASSWORD);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        if (millis() - start > TIMEOUT_MS) {
            Serial.println("Could not get connection to wifi (timeout)");
            Serial.println(MYSSID);
            Serial.println(WiFi.macAddress());
            return false;
        }
        delay(400);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }

    for (int i = 0; i < 5; i++) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
    }
    Serial.println("Connected to wifi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    wifi.setInsecure();
    return true;
}

void disconnectWifi() {
    WiFi.disconnect(true);
}


void addDataToBuf(GameState& game) {
    // Create JSON document for single game
    JsonDocument doc;

    // Add device identification
    doc["device_id"] = WiFi.macAddress();
    doc["timestamp"] = millis();

    // Add game results
    doc["level_reached"] = game.currentLevel;

    // Add timing array
    JsonArray timings = doc["response_times"].to<JsonArray>();
    for (int i = 0; i < game.currentLevel; i++) {
        timings.add(game.timeSpent[i]);
    }

    // Serialize to string
    std::string jsonString;
    serializeJson(doc, jsonString);
    Serial.print("Serialized game data: ");
    Serial.println(jsonString.c_str());

    // Add to buffer
    std::lock_guard<std::mutex> lock(bufferMutex);
    if (gameDataBuffer.size() >= MAX_GAMES_IN_BUFFER) {
        // Remove oldest game if buffer full
        gameDataBuffer.pop();
        Serial.println("Warning: Buffer full, removing oldest game data");
    }

    gameDataBuffer.push(jsonString);
    Serial.print("Game data added to buffer. Buffer size: ");
    Serial.println(gameDataBuffer.size());
}


bool transmitData() {
    // Obtain data from buffer with locking
    std::string payload = "";
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        if (gameDataBuffer.empty()) {
            return false;
        }

        // Get the oldest game data
        payload = gameDataBuffer.front();
        gameDataBuffer.pop();
    } // Release lock

    // Send HTTP POST
    http.begin(wifi, SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

    int httpResponseCode = http.POST(payload.c_str());

    if (httpResponseCode >= 200 && httpResponseCode < 300) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.print("Successfully transmitted ");
        Serial.print(gameDataBuffer.size());
        Serial.println(" game sessions");
        http.end();
    }
    else {
        Serial.print("Error sending POST: ");
        Serial.println(httpResponseCode);
        std::lock_guard<std::mutex> lock(bufferMutex);
        gameDataBuffer.push(payload); // Re-add data to buffer for retry
        Serial.println("Data retained in buffer for retry");
    }

    http.end();
    return true;
}
