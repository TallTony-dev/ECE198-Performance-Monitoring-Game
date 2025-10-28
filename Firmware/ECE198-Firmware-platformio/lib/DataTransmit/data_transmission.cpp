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

WiFiClient wifi;
int status = WL_IDLE_STATUS;
HTTPClient http;

// Buffer to store up to 20 game sessions as JSON strings
constexpr int MAX_GAMES_IN_BUFFER = 20;
std::mutex bufferMutex;
std::queue<std::string> gameDataBuffer;

bool connectToWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start > TIMEOUT_MS) {
            Serial.println("Could not get connection to wifi (timeout)");
            Serial.println(SSID);
            Serial.println(WiFi.macAddress());
            return false;
        }
        delay(200);
    }
    Serial.println("Connected to wifi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
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
    JsonArray timings = doc.createNestedArray("response_times");
    for (int i = 0; i < game.currentLevel; i++) {
        timings.add(game.timeSpent[i]);
    }

    // Serialize to string
    std::string jsonString;
    serializeJson(doc, jsonString);

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


void transmitData() {
    std::string payload = "";
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        if (gameDataBuffer.empty()) {
            Serial.println("No data to transmit");
            return;
        }

        // Get the first
        payload = gameDataBuffer.front();
        gameDataBuffer.pop();
    }

    // Send HTTP POST
    http.begin(wifi, SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload.c_str());

    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.print("Successfully transmitted ");
        Serial.print(gameDataBuffer.size());
        Serial.println(" game sessions");
    }
    else {
        Serial.print("Error sending POST: ");
        Serial.println(httpResponseCode);
        Serial.println("Data retained in buffer for retry");
    }

    http.end();
}
