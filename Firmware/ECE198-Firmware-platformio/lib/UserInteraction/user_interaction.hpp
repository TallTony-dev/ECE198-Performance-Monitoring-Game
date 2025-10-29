// Hardware Pin Definitions
#define SPEAKER_PIN 25      ///< PWM pin for audio output

#define LED_PIN_1 27        ///< LED for button 1 feedback
#define LED_PIN_2 14        ///< LED for button 2 feedback
#define LED_PIN_3 12        ///< LED for button 3 feedback
#define LED_PIN_4 13        ///< LED for button 4 feedback

#define BUTTON_PIN_1 19     ///< Input pin for button 1
#define BUTTON_PIN_2 18     ///< Input pin for button 2
#define BUTTON_PIN_3 5      ///< Input pin for button 3
#define BUTTON_PIN_4 17     ///< Input pin for button 4

// Audio Tone Frequencies (Hz)
#define ERROR_TONE 500      ///< Error indication tone
#define TONE_1 1000         ///< Button 1 tone
#define TONE_2 1500         ///< Button 2 tone
#define TONE_3 2000         ///< Button 3 tone
#define TONE_4 2400         ///< Button 4 tone


// Input Debouncing
#define DEBOUNCE_MS 20L     ///< Debounce delay in milliseconds

/**
 * @brief Activate LED and play tone simultaneously
 *
 * @param LEDPin GPIO pin for LED
 * @param freq Tone frequency in Hz
 * @param durationMS Duration in milliseconds
 */
void output(int LEDPin, int freq, int durationMS);

/**
 * @brief Check single button with debouncing and feedback
 *
 * @param buttonPin GPIO pin to check
 * @param ledPin LED pin for visual feedback
 * @param tone Audio frequency for feedback
 * @return true if button pressed and debounced
 */
bool checkButtonInput(const int buttonPin, const int ledPin, const int tone);

/**
 * @brief Scan all buttons and return which was pressed
 *
 * @return Button number (1-4) if pressed, 0 if none
 */
int checkInputs();

/**
 * @brief Play audio tone through speaker
 *
 * @param freq Frequency in Hz
 * @param durationMS Duration in milliseconds
 */
void playSound(int freq, int durationMS);

/**
 * @brief Flash LED for specified duration
 *
 * @param pin GPIO pin
 * @param durationMS Duration in milliseconds
 */
void blinkLED(int pin, int durationMS);

/**
 * @brief Display error feedback (all LEDs + error tone)
 *
 * Activates all LEDs and plays error tone for 2 seconds
 */
void incorrectAnswer();