#pragma once
// Hardware Pin Definitions

/*
 * ==========================================================================================
 * GPIO SAFETY WARNING
 * ==========================================================================================
 * WARNING: Ensure to avoid high-risk pins to prevent program crashing or upload failure.
 *
 * 1. AVOID GPIO 1 (U0TXD) and GPIO 3 (U0RXD) because they interfere with Serial Monitor
 * communication and can prevent debugging.
 * 2. AVOID GPIO 6 through 11 because they are connected to the internal SPI Flash memory
 * and should never be used.
 * 3. AVOID GPIO 0, 2, 5, 12, and 15 because they are Strapping Pins, meaning their state
 * at boot-up can force the chip into an unstable mode (like Flashing Mode).
 *
 * Recommended Safe Pins to Use: 4, 13, 14, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33
 * Input-Only Pins (Safe for sensors): 34, 35, 36, 39
 * ==========================================================================================
 */


 // Change this value to switch between your boards
#define BOARD_ID 'A'

#if BOARD_ID == 'A' // Yiran's prototype board
#define SPEAKER_PIN     4

#define LED_PIN_1       32 
#define LED_PIN_2       33
#define LED_PIN_3       25
#define LED_PIN_4       26

#define BUTTON_PIN_1    27 
#define BUTTON_PIN_2    14 
#define BUTTON_PIN_3    12  
#define BUTTON_PIN_4    13
#endif

#if BOARD_ID == 'B' // Tony's prototype board
#define SPEAKER_PIN     25

#define LED_PIN_1       27
#define LED_PIN_2       14
#define LED_PIN_3       12
#define LED_PIN_4       13

#define BUTTON_PIN_1    19
#define BUTTON_PIN_2    18
#define BUTTON_PIN_3    5
#define BUTTON_PIN_4    17
#endif

// Audio Tone Frequencies (Hz)
#define ERROR_TONE 500      ///< Error indication tone
#define TONE_1 1000         ///< Button 1 tone
#define TONE_2 1500         ///< Button 2 tone
#define TONE_3 2000         ///< Button 3 tone
#define TONE_4 2400         ///< Button 4 tone

// Light showing speed
#define LIGHTUP_MS 400

// Input Debouncing
#define DEBOUNCE_MS 20     ///< Debounce delay in milliseconds

/**
 * @brief Activate LED and play tone simultaneously
 *
 * @param LEDPin GPIO pin for LED
 * @param freq Tone frequency in Hz
 */
void output(int LEDPin, int freq);

/**
 * @brief Check single button with debouncing and feedback
 *
 * @param buttonPin GPIO pin to check
 * @param ledPin LED pin for visual feedback
 * @param freq Audio frequency for feedback
 * @return true if button pressed and debounced
 */
bool checkButtonInput(const int buttonPin, const int LEDPin, const int freq);

/**
 * @brief Scan all buttons and return which was pressed
 *
 * @return Button number (1-4) if pressed, 0 if none
 */
int checkInputs();

/**
 * @brief Display error feedback (all LEDs + error tone)
 *
 * Activates all LEDs and plays error tone for 2 seconds
 */
void incorrectAnswer();