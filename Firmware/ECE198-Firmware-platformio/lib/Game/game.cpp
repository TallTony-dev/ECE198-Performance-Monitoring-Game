/**
 * @file game.cpp
 * @brief Implementation of Simon Says memory game logic
 * @author TallTony-dev catr1xLiu
 */

#include "game.hpp"
#include "user_interaction.hpp"
#include <Arduino.h>
#include <sys/time.h>

void initGame(GameState& state) {
    state.currentLevel = 1;
    state.timeSpent.fill(0);
    state.colorSequence.fill(0);
}

void displaySequence(const GameState& state) {
    for (int i = 0; i < state.currentLevel; i++) {
        if (state.colorSequence[i] == 1) {
            output(LED_PIN_1, TONE_1, 1000);
        }
        else if (state.colorSequence[i] == 2) {
            output(LED_PIN_2, TONE_2, 1000);
        }
        else if (state.colorSequence[i] == 3) {
            output(LED_PIN_3, TONE_3, 1000);
        }
        else if (state.colorSequence[i] == 4) {
            output(LED_PIN_4, TONE_4, 1000);
        }

        // Add gap between elements (except after last element)
        if (i != state.currentLevel - 1) {
            delay(100);
        }
    }
}

bool getPlayerInput(const GameState& state, int& deltaTimeSeconds) {
    timeval startTime;
    gettimeofday(&startTime, NULL);

    // Get user inputs and validate
    for (int i = 0; i < state.currentLevel; i++) {
        int answer = 0;

        // Wait for button press
        while (!(answer = checkInputs())) {}

        // Check if answer is correct
        if (answer != state.colorSequence[i]) {
            incorrectAnswer();

            // Calculate time even on failure
            timeval endTime;
            gettimeofday(&endTime, NULL);
            deltaTimeSeconds = endTime.tv_sec - startTime.tv_sec;

            return false;
        }
    }

    // Calculate completion time
    timeval endTime;
    gettimeofday(&endTime, NULL);
    deltaTimeSeconds = endTime.tv_sec - startTime.tv_sec;

    return true;
}

bool playRound(GameState& state) {
    // Generate next element in sequence
    int randomButton = random(1, 5); // Random between 1 and 4 inclusive
    state.colorSequence[state.currentLevel - 1] = randomButton;

    // Display the sequence
    displaySequence(state);

    // Get and validate player input
    int deltaTime = 0;
    bool isCorrect = getPlayerInput(state, deltaTime);

    // Store timing data
    state.timeSpent[state.currentLevel - 1] = deltaTime;

    if (isCorrect && state.currentLevel < MAX_LEVEL) {
        // Advance to next level
        state.currentLevel++;
        delay(1000); // Brief pause before next round
        return true; // Continue game
    }
    else {
        // Game ended (either failed or completed all levels)
        return false;
    }
}

void resetGame(GameState& state) {
    initGame(state);
}