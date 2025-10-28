#pragma once

#include <array>
#include <cstdint>

#define MAX_LEVEL 10

/**
 * @brief Game state data structure
 *
 * Holds all data needed to track game progress including
 * current level, button sequence, and response times.
 */
struct GameState {
    int currentLevel;                         ///< Current difficulty level (1-MAX_LEVEL)
    std::array<int, MAX_LEVEL> timeSpent;        ///< Response times per level (seconds)
    std::array<int, MAX_LEVEL> colorSequence;     ///< Button sequence to memorize (1-4)
};

/**
 * @brief Initialize game state to starting values
 *
 * Resets level to 1 and clears all timing and sequence data.
 *
 * @param[out] state Game state structure to initialize
 *
 * @note Call before starting new game session
 *
 * @code
 * GameState game;
 * initGame(game);
 * @endcode
 */
void initGame(GameState& state);

/**
 * @brief Execute one round of the game
 *
 * Generates sequence, displays it, gets player input, and
 * updates game state based on correctness.
 *
 * @param[in,out] state Game state to update
 * @return true if game should continue, false if game ended
 *
 * @details
 * Game Flow:
 * 1. Generate random button for current position
 * 2. Display full sequence to player
 * 3. Wait for player input
 * 4. Validate input and track timing
 * 5. Advance level or end game
 *
 * @code
 * GameState game;
 * initGame(game);
 * while (playRound(game)) {
 *     // Continue playing
 * }
 * @endcode
 */
bool playRound(GameState& state);

/**
 * @brief Display the current sequence to the patient
 *
 * Shows LED and audio pattern for each element in the sequence
 * up to the current level.
 *
 * @param[in] state Game state containing sequence to display
 *
 * @note Uses 1 second duration per element with 100ms gap
 * @warning Blocking function - uses delay()
 */
void displaySequence(const GameState& state);

/**
 * @brief Get and validate player input for current level
 *
 * Waits for player to input the complete sequence and validates
 * each button press against the expected sequence.
 *
 * @param[in] state Game state containing expected sequence
 * @param[out] deltaTimeSeconds Time taken to complete sequence
 * @return true if sequence entered correctly, false otherwise
 *
 * @note Blocking function - waits for all inputs
 * @warning Does not timeout - will wait indefinitely for input
 */
bool getPlayerInput(const GameState& state, int& deltaTimeSeconds);

/**
 * @brief Reset game to initial state
 *
 * Clears all progress and returns to level 1.
 * Used after game completion or when starting new session.
 *
 * @param[out] state Game state to reset
 *
 * @note Functionally equivalent to initGame()
 */
void resetGame(GameState& state);
