#define SPEAKER_PIN 25
#define LED_PIN_1 27
#define LED_PIN_2 14
#define LED_PIN_3 12
#define LED_PIN_4 13

#define BUTTON_PIN_1 19
#define BUTTON_PIN_2 18
#define BUTTON_PIN_3 5
#define BUTTON_PIN_4 17

#define ERROR_TONE 500
#define TONE_1 1000
#define TONE_2 1500
#define TONE_3 2000
#define TONE_4 2400

#define MAX_LEVEL 50 //i know this shouldnt be here :P

void Output(int LEDPin, int freq, int durationMS);
int CheckInputs(); //returns the index of button pressed and outputs relevant light and sound for button
void PlaySound(int freq, int durationMS);
void BlinkLED(int pin, int durationMS);
void IncorrectAnswer();