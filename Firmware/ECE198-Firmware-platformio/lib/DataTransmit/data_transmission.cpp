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
#include <vector>
#include <string>

WiFiClient wifi;
int status = WL_IDLE_STATUS;
HTTPClient http;

// Buffer to store up to 20 game sessions as JSON strings
constexpr int MAX_GAMES_IN_BUFFER = 20;
std::vector<std::string> gameDataBuffer;

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
    if (gameDataBuffer.size() >= MAX_GAMES_IN_BUFFER) {
        // Remove oldest game if buffer full
        gameDataBuffer.erase(gameDataBuffer.begin());
        Serial.println("Warning: Buffer full, removing oldest game data");
    }

    gameDataBuffer.push_back(jsonString);
    Serial.print("Game data added to buffer. Buffer size: ");
    Serial.println(gameDataBuffer.size());
}


void transmitData() {
    if (gameDataBuffer.empty()) {
        Serial.println("No data to transmit");
        return;
    }

    // Create JSON array containing all game sessions
    JsonDocument doc;
    JsonArray gamesArray = doc.to<JsonArray>();

    // Parse each stored JSON string and add to array
    for (const auto& jsonStr : gameDataBuffer) {
        JsonDocument gameDoc;
        deserializeJson(gameDoc, jsonStr);
        gamesArray.add(gameDoc);
    }

    // Serialize final payload
    String payload;
    serializeJson(doc, payload);

    // Send HTTP POST
    http.begin(wifi, SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.print("Successfully transmitted ");
        Serial.print(gameDataBuffer.size());
        Serial.println(" game sessions");

        // Clear buffer on success
        gameDataBuffer.clear();
    }
    else {
        Serial.print("Error sending POST: ");
        Serial.println(httpResponseCode);
        Serial.println("Data retained in buffer for retry");
    }

    http.end();
}
