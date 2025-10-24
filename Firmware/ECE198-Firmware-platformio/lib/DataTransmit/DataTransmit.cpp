#include <HTTPClient.h>
#ifndef SPEAKER_PIN
#include "GPIOStuff.hpp"
#endif
#include <string>

HTTPClient client;
String url = "https://<IPaddress>/testurl"; //TODO: replace with real address

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
        for (int i = 0; i < memoryLength; i++) {
            newBuffer[i] = Buffer[i];
        }
        memoryLength += 40;
        if (newBuffer != nullptr) {
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

//partly paraphrased from https://stackoverflow.com/questions/3677400/making-a-http-post-request-using-arduino
void TransmitData() {
    client.begin(url);

    client.addHeader("Content-Type", "application/octet-stream");
    client.addHeader("Connection", "close");
    client.addHeader("Content-Length", String(currentBufferIndex));

    int httpResponseCode = client.POST(Buffer, currentBufferIndex);

    if (httpResponseCode > 0) {
        String response = client.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
    } 
    else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
    }
    client.end();
}