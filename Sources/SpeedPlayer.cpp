#include "../Headers/Players/SpeedPlayer.hpp"
#include "../Headers/Coins/Coin.hpp"
#include <vector>
#include <algorithm> 
#include "raylib.h"

#define PLAYER 'P'
#define ENEMY1 '1'
#define ENEMY2 '2'
#define BIG_ENEMY 'B'
#define WALL 'W'
#define Key 'K'
#define COIN 'C'
#define Special_Coin 'S'

SpeedPlayer::SpeedPlayer() {}

SpeedPlayer::SpeedPlayer(string name, int health, int attack, int num_of_fights, int current_score) {
    this->name = name;
    this->health = health;
    this->attack = attack;
    this->num_of_fights = num_of_fights;
    this->current_score = current_score;
    this->player_movement_sound = LoadSound("Resources\\player_movement.mp3"); // Load player movement sound
}

int SpeedPlayer::move(vector<vector<vector<pair<char, void*>>>>& map, Room* room) {
    int PosX = position.first;
    int PosY = position.second;

    int coin_score = -1;

    // Define movement directions
    vector<pair<int, int>> directions = {
        {-1, 0}, // Up
        {1, 0},  // Down
        {0, -1}, // Left
        {0, 1}   // Right
    };

    // Define corresponding keys
    vector<int> keys = {
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT
    };

    // Iterate over possible directions
    for (int i = 0; i < directions.size(); ++i) {
        if (IsKeyPressed(keys[i])) {
            int newX1 = PosX + directions[i].first;  // First block
            int newY1 = PosY + directions[i].second;
            int newX2 = newX1 + directions[i].first; // Second block
            int newY2 = newY1 + directions[i].second;

            // Check if the first block is within bounds
            if (newX1 >= 0 && newX1 < map.size() && newY1 >= 0 && newY1 < map[0].size()) {
                auto& movePos1 = map[newX1][newY1];

                // anonymous function to check if the cell is empty or contains a coin
                auto isEmptyOrCoin = [&movePos1]() {
                    return std::all_of(movePos1.begin(), movePos1.end(), [](const pair<char, void*>& p) {
                        return p.first == COIN || p.first == Special_Coin || p.first == Key;
                    }) || movePos1.empty();
                };

                // Check if the first block is empty or contains a coin
                if (isEmptyOrCoin()) {
                    // Check if the second block is within bounds
                    if (newX2 >= 0 && newX2 < map.size() && newY2 >= 0 && newY2 < map[0].size()) {
                        auto& movePos2 = map[newX2][newY2];

                        // anonymous function to check if the cell is empty or contains a coin
                        auto isEmptyOrCoin = [&movePos2]() {
                            return std::all_of(movePos2.begin(), movePos2.end(), [](const pair<char, void*>& p) {
                                return p.first == COIN || p.first == Special_Coin || p.first == Key;
                            }) || movePos2.empty();
                        };

                        // Check if the second block is empty or contains a coin
                        if (isEmptyOrCoin()) {
                            // Remove the player from the old position
                            auto& currentCell = map[PosX][PosY];
                            auto it = std::find_if(currentCell.begin(), currentCell.end(),
                                [this](const std::pair<char, void*>& p) {
                                    return (p.first == PLAYER) && (p.second == static_cast<void*>(this));
                                });
                            if (it != currentCell.end()) {
                                currentCell.erase(it);
                            }

                            // Move two blocks ahead
                            position = make_pair(newX2, newY2);
                            map[newX2][newY2].emplace_back(make_pair(PLAYER, this));
                            PlaySound(player_movement_sound); // Play player movement sound

                            // Check for coins in the skipped cell (first block)
                            for (auto it = movePos1.begin(); it != movePos1.end(); ++it) {
                                if (it->first == COIN || it->first == Special_Coin) {
                                    // Add the coin value to the player's score
                                    current_score += static_cast<Coin*>(it->second)->getScore();

                                    coin_score = static_cast<Coin*>(it->second)->getScore(); // Get the coin score

                                    // Remove and delete the coin since it is dynamically allocated
                                    delete static_cast<Coin*>(it->second);
                                    movePos1.erase(it);
                                    break; // Exit after processing the coin
                                }
                            }

                            // Check if the second block has a coin
                            for (auto it = movePos2.begin(); it != movePos2.end(); ++it) {
                                if (it->first == COIN || it->first == Special_Coin) {
                                    // Add the coin value to the player's score
                                    current_score += static_cast<Coin*>(it->second)->getScore();

                                    coin_score = static_cast<Coin*>(it->second)->getScore(); // Get the coin score

                                    // Remove and delete the coin since it is dynamically allocated
                                    delete static_cast<Coin*>(it->second);
                                    movePos2.erase(it);
                                    break; // Exit after processing the coin
                                } else if (it->first == Key) {
                                    // Handle key collection
                                    movePos2.erase(it); // Remove the key from the map
                                    room->is_key_collected = true; // Set the key collected flag to true
        
                                    break; // Exit the loop after handling the key
                                }
                            }
                            break; // Exit after moving two blocks
                        }
                    }

                    // If the second block is not valid, move one block ahead
                    // Remove the player from the old position
                    auto& currentCell = map[PosX][PosY];
                    auto it = std::find_if(currentCell.begin(), currentCell.end(),
                        [this](const std::pair<char, void*>& p) {
                            return (p.first == PLAYER) && (p.second == static_cast<void*>(this));
                        });
                    if (it != currentCell.end()) {
                        currentCell.erase(it);
                    }

                    position = make_pair(newX1, newY1);
                    map[newX1][newY1].push_back(make_pair(PLAYER, this));
                    PlaySound(player_movement_sound); // Play player movement sound

                    // Check if the first block has a coin
                    for (auto it = map[newX1][newY1].begin(); it != map[newX1][newY1].end(); ++it) {
                        if (it->first == COIN || it->first == Special_Coin) {
                            // Add the coin value to the player's score
                            current_score += static_cast<Coin*>(it->second)->getScore();

                            coin_score = static_cast<Coin*>(it->second)->getScore(); // Get the coin score

                            // Remove and delete the coin since it is dynamically allocated
                            delete static_cast<Coin*>(it->second);
                            map[newX1][newY1].erase(it);
                            break; // Exit after processing the coin
                        } else if (it->first == Key) {
                            // Handle key collection
                            movePos1.erase(it); // Remove the key from the map
                            room->is_key_collected = true; // Set the key collected flag to true

                            break; // Exit the loop after handling the key
                        }
                    }   
                    break; // Exit after moving one block
                }
                // Check if the first block is a wall
                else if (movePos1.size() > 0 && movePos1[0].first == WALL) {
                    // Do nothing, player cannot move into a wall
                }
                // Check if the first block is an enemy
                else if (movePos1.size() > 0 && (movePos1[0].first == ENEMY1 || movePos1[0].first == ENEMY2 || movePos1[0].first == BIG_ENEMY)) {
                    // Handle enemy collision
                    break; // Exit after handling enemy collision
                }
            }
        }
    }

    return coin_score; // Return the score of the coin if collected, otherwise return -1
}

SpeedPlayer::~SpeedPlayer() {
    UnloadSound(player_movement_sound); // Unload player movement sound
}
