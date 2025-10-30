/**
 * @file main.ino
 * @brief Main entry point for HydroGuard cognitive assessment system
 *
 * Manages system initialization, WiFi connectivity, game execution,
 * data transmission, and power management.
 *
 * @author TallTony-dev
 */

#include <Arduino.h>
#include "data_transmission.hpp"
#include "user_interaction.hpp"
#include "game.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define SLEEPPIN GPIO_NUM_32  ///< GPIO pin for wake-up from deep sleep

 // Forward declarations
void deepSleep();
void dataTransmissionLoop(void* parameter);

/**
 * @brief Initialize system hardware and peripherals
 *
 * Configures serial, WiFi, GPIO pins, and audio output.
 * Blocks until WiFi connection established.
 */
void setup() {
    Serial.begin(115200);

    delay(2000);
    Serial.println("Setup running...");

    // Configure speaker PWM
    ledcSetup(0, 2000, 8);
    ledcAttachPin(SPEAKER_PIN, 1);

    // Connect to WiFi
    do {
        Serial.print("Connecting to wifi...");
    } while (!connectToWifi());

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

    Serial.println("Setup complete");
    delay(500);
    xTaskCreatePinnedToCore(
        dataTransmissionLoop,   // Task function
        "DataTransmission",     // Name of task
        8192,                   // Stack size
        NULL,                   // Parameter to pass
        1,                      // Task priority
        NULL,                   // Task handle
        1                       // Run on core 1
    );
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
        Serial.println("Level completed! Advancing to next level...");
    }

    Serial.println("Game over!");
    Serial.print("Levels completed: ");
    Serial.println(game.currentLevel);

    // Upload performance data to cloud
    addDataToBuf(game);  // Simplified - just pass the game state
    deepSleep();
}

/**
 * @brief Data transmission loop task
 *
 * Continuously attempts to transmit buffered data to cloud server.
 * Runs as a FreeRTOS task.
 */
void dataTransmissionLoop(void* parameter) {
    while (true) {
        if (transmitData()) {
            continue;
        }
        delay(1000);  // Wait before next transmission
    }
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