#include <future>
#include "GPIOStuff.hpp"
#include <Arduino.h>

int CheckInputs() {
    const unsigned long debounceMs = 20;
    if (digitalRead(BUTTON_PIN_1) == LOW) {
        delay(debounceMs);
        if (digitalRead(BUTTON_PIN_1) == LOW) {
            Output(LED_PIN_1, TONE_1, 200);
            return 1;
        }
    }
    if (digitalRead(BUTTON_PIN_2) == LOW) {
        delay(debounceMs);
        if (digitalRead(BUTTON_PIN_2) == LOW) {
            Output(LED_PIN_2, TONE_2, 200);
            return 2;
        }
    }
    if (digitalRead(BUTTON_PIN_3) == LOW) {
        delay(debounceMs);
        if (digitalRead(BUTTON_PIN_3) == LOW) {
            Output(LED_PIN_3, TONE_3, 200);
            return 3;
        }
    }
    if (digitalRead(BUTTON_PIN_4) == LOW) {
        delay(debounceMs);
        if (digitalRead(BUTTON_PIN_4) == LOW) {
            Output(LED_PIN_4, TONE_4, 200);
            return 4;
        }
    }
    return 0;
}

//could use DAC onboard if sound doesnt suffice
void PlaySound(int freq, int durationMS) {
    noTone(SPEAKER_PIN);
    tone(SPEAKER_PIN, freq);
    delay(durationMS);
    noTone(SPEAKER_PIN);
}

void BlinkLED(int pin, int durationMS) {
    digitalWrite(pin, HIGH);
    delay(durationMS);
    digitalWrite(pin, LOW);
}

void Output(int LEDPin, int freq, int durationMS) {
    noTone(SPEAKER_PIN);
    digitalWrite(LEDPin, HIGH);
    tone(SPEAKER_PIN, freq);
    delay(durationMS);
    noTone(SPEAKER_PIN);
    digitalWrite(LEDPin, LOW);
}

void IncorrectAnswer() {
    //make sounds and turn on and blink lights
    noTone(SPEAKER_PIN);
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, HIGH);
    digitalWrite(LED_PIN_3, HIGH);
    digitalWrite(LED_PIN_4, HIGH);
    tone(SPEAKER_PIN, ERROR_TONE);
    delay(2000);
    noTone(SPEAKER_PIN);
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, LOW);
    digitalWrite(LED_PIN_4, LOW);

    //maybe play a little tune here too for fun
}