#include <Arduino.h>
#include "user_interaction.hpp"

int checkInputs() {
    if (checkButtonInput(BUTTON_PIN_1, LED_PIN_1, TONE_1)) {
        return 1;
    }
    if (checkButtonInput(BUTTON_PIN_2, LED_PIN_2, TONE_2)) {
        return 2;
    }
    if (checkButtonInput(BUTTON_PIN_3, LED_PIN_3, TONE_3)) {
        return 3;
    }
    if (checkButtonInput(BUTTON_PIN_4, LED_PIN_4, TONE_4)) {
        return 4;
    }

    delay(DEBOUNCE_MS);
    return 0;
}

bool checkButtonInput(const int buttonPin, const int LEDPin, const int freq)
{
    bool result = false;
    if (digitalRead(buttonPin) == HIGH) {
        return false;
    }

    digitalWrite(LEDPin, HIGH);
    tone(SPEAKER_PIN, freq);
    result = false;
    delay(DEBOUNCE_MS);
    while (digitalRead(buttonPin) == LOW) {
        delay(DEBOUNCE_MS);
        result = true;
    }
    noTone(SPEAKER_PIN);
    digitalWrite(LEDPin, LOW);
    return result;
}

void output(int LEDPin, int freq) {
    noTone(SPEAKER_PIN);
    digitalWrite(LEDPin, HIGH);
    tone(SPEAKER_PIN, freq);
    delay(LIGHTUP_MS);
    noTone(SPEAKER_PIN);
    digitalWrite(LEDPin, LOW);
}

void incorrectAnswer() {
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