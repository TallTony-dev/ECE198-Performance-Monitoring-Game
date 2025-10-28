#include <Arduino.h>
#include "data_transmission.hpp"
#include "user_interaction.hpp"
#include "game.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define SLEEPPIN GPIO_NUM_32  ///< GPIO pin for wake-up from deep sleep

// Forward declarations
void deepSleep();

/**
 * @brief Initialize system hardware and peripherals
 *
 * Configures serial, WiFi, GPIO pins, and audio output.
 * Blocks until WiFi connection established.
 */
void setup() {
    Serial.begin(9600);

    // Configure speaker PWM
    ledcSetup(0, 2000, 8);
    ledcAttachPin(SPEAKER_PIN, 1);

    // Connect to WiFi
    while (!connectToWifi()) {
        Serial.println("Connecting to wifi...");
    }

    // Configure status LED
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);

    // Configure game interface pins
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    pinMode(LED_PIN_3, OUTPUT);
    pinMode(LED_PIN_4, OUTPUT);
    pinMode(BUTTON_PIN_1, INPUT_PULLUP);
    pinMode(BUTTON_PIN_2, INPUT_PULLUP);
    pinMode(BUTTON_PIN_3, INPUT_PULLUP);
    pinMode(BUTTON_PIN_4, INPUT_PULLUP);
    pinMode(SPEAKER_PIN, OUTPUT);
}

/**
 * @brief Main game loop
 *
 * Runs cognitive assessment game, transmits results to cloud,
 * then enters deep sleep mode.
 */
void loop() {
    GameState game;
    initGame(game);

    // Play game until completion or failure
    while (playRound(game)) {
        // Continue playing rounds
    }

    // Upload performance data to cloud
    for (int i = 0; i < game.currentLevel; i++) {
        int time = game.timeSpent[i];
        addDataToBuf(reinterpret_cast<uint8_t*>(&time), sizeof(time));
    }

    // Add final level reached
    addDataToBuf(reinterpret_cast<uint8_t*>(&game.currentLevel), sizeof(int));

    // Transmit data and enter sleep mode
    transmitData();
    disconnectWifi();
    deepSleep();
}

/**
 * @brief Enter deep sleep with button wake-up
 *
 * Configures ESP32 to wake when SLEEPPIN goes LOW.
 * System restarts from setup() on wake.
 */
void deepSleep() {
    Serial.println("Sleeping...");

    esp_sleep_enable_ext0_wakeup(SLEEPPIN, 0); // Wake on LOW
    pinMode(SLEEPPIN, INPUT_PULLUP);
    gpio_deep_sleep_hold_en();

    Serial.flush();
    delay(200);
    esp_deep_sleep_start();
}