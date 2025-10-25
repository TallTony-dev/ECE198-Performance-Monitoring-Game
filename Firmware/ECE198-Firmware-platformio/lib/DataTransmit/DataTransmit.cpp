#include <HTTPClient.h>
#ifndef SPEAKER_PIN
#include "GPIOStuff.hpp"
#endif
#include <string>

char ssid[] = "UniversityOfWaterloo"; //SSID/name
//char pass[] = "grahwahh"; //password
char url[] =/*"http://webhook.site/2128266b-96bc-4ee1-ab6f-751a862ec269"*/"https://ece-198-performance-monitoring-game-seven.vercel.app/api/apitest"; //url to post to 

WiFiClient wifi;
int status = WL_IDLE_STATUS;
HTTPClient http;


uint8_t *Buffer = new uint8_t[50]; //TODO: ADD MEMORY MANAGEMENT
int memoryLength = 50;
int currentBufferIndex = 0;

void AddCharToBuf(char c) {
    if (currentBufferIndex < memoryLength) {
        Buffer[currentBufferIndex] = c;
        currentBufferIndex++;
    }
    else {
        uint8_t *newBuffer = new uint8_t[memoryLength + 40];
        for (int i = 0; i < currentBufferIndex; i++) {
            newBuffer[i] = Buffer[i];
        }
        if (newBuffer != nullptr) {
            memoryLength += 40;
            delete[] Buffer;
            Buffer = newBuffer;
            Buffer[currentBufferIndex] = c;
            currentBufferIndex++;
        }
        else {
            Serial.println("Error: Failed Allocation");
        }
    }
}

//Delmits data with comma appended to end
void AddDataToBuf(uint8_t* data, int length) { //length in bytes
    for (int i = 0; i < length; i++) {
        AddCharToBuf(data[i]);
    }
    AddCharToBuf(',');
}

//call in initialization, returns true if connected
bool ConnectToWifi(unsigned long timeoutMs = 2000) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid /*, pass*/);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start > timeoutMs) {
            Serial.println("Could not get connection to wifi (timeout)");
            Serial.println(ssid);
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


//partly paraphrased from https://stackoverflow.com/questions/3677400/making-a-http-post-request-using-arduino
void TransmitData() {
    if (WiFi.status() != WL_CONNECTED) {
        while (!ConnectToWifi()) {
            Serial.println("wifi disconnected, attempting to reconnect");
        }
    }

    if(!http.begin(wifi, url)) { 
        Serial.println("http connection to server failed, giving up");
        return;
    }

    http.addHeader("Content-Type", "application/octet-stream");
    http.addHeader("Connection", "close");
    http.addHeader("Content-Length", String(currentBufferIndex));

    Serial.println(WiFi.status());
    Serial.println(currentBufferIndex);

    int httpResponseCode = http.POST(Buffer, currentBufferIndex);

    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.print("response: ");
        Serial.println(response);
        currentBufferIndex = 0;
    } 
    else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}