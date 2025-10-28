/**
 * @file data_transmission.hpp
 * @brief IoT data transmission interface for HydroGuard
 * @author caTr1xLiu
 */

#pragma once

#include "game.hpp"

 /**
  * @brief Add game results to transmission buffer
  *
  * Serializes single game session into JSON and adds to buffer.
  * Buffer holds maximum 20 game sessions.
  *
  * @param[in] game Game state containing timing and level data
  *
  * @note If buffer full, oldest entry is removed
  */
void addDataToBuf(GameState& game);

/**
 * @brief Transmit buffered data to cloud server
 *
 * Sends all buffered game sessions as JSON array via HTTP POST.
 * Clears buffer after successful transmission.
 *
 * @note Requires active WiFi connection
 */
void transmitData();

/**
 * @brief Establish WiFi connection
 *
 * Attempts to connect to configured WiFi network.
 *
 * @return true if connection established, false on timeout
 *
 * @note Times out after 10 seconds if unable to connect
 */
bool connectToWifi();

/**
 * @brief Disconnect from WiFi network
 *
 * Closes WiFi connection to conserve power before deep sleep.
 */
void disconnectWifi();