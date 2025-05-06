#include "../Headers/Players/HealthPlayer.hpp"
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

HealthPlayer::HealthPlayer() {}

HealthPlayer::HealthPlayer(string name, int health, int attack, int num_of_fights, int current_score) {
    this->name = name;
    this->health = health;
    this->attack = attack;
    this->num_of_fights = num_of_fights;
    this->current_score = current_score;
    this->player_movement_sound = LoadSound("Resources\\player_movement.mp3"); // Load player movement sound
}

int HealthPlayer::move(vector<vector<vector<pair<char, void*>>>>& map, Room* room) {
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

    for (int i = 0; i < directions.size(); ++i) {
        if (IsKeyPressed(keys[i])) {
            int newX = PosX + directions[i].first;
            int newY = PosY + directions[i].second;

            // Check if the new position is within bounds
            if (newX >= 0 && newX < map.size() && newY >= 0 && newY < map[0].size()) {
                auto& movePos = map[newX][newY];

                // anonymous function to check if the cell is empty or contains a coin
                auto isEmptyOrCoin = [&movePos]() {
                    return std::all_of(movePos.begin(), movePos.end(), [](const pair<char, void*>& p) {
                        return p.first == COIN || p.first == Special_Coin || p.first == Key;
                    }) || movePos.empty();
                };

                // Check if the move position is empty or contains a coin
                if (isEmptyOrCoin()) {
                    // Set the player position to the move position
                    position = make_pair(newX, newY);
                    map[newX][newY].push_back(make_pair(PLAYER, this));
                    PlaySound(player_movement_sound); // Play player movement sound

                    // auto it = find(map[PosX][PosY].begin(), map[PosX][PosY].end(), make_pair(PLAYER, this));
                    auto& currentCell = map[PosX][PosY];
                    auto it = std::find_if(currentCell.begin(), currentCell.end(), 
                        [this](const pair<char, void*>& p) {
                        return p.first == PLAYER && p.second == static_cast<void*>(this);
                    });
                    if (it != currentCell.end()) {
                        currentCell.erase(it);
                    }

                    // Iterate through the move position to check for a coin
                    for (auto it = movePos.begin(); it != movePos.end(); ++it) {
                        if (it->first == COIN || it->first == Special_Coin) {
                            // Add the coin value to the player's score
                            current_score += static_cast<Coin*>(it->second)->getScore();

                            coin_score = static_cast<Coin*>(it->second)->getScore(); // Get the coin score

                            // Remove and delete the coin since it is dynamically allocated
                            delete static_cast<Coin*>(it->second); // Free memory
                            map[newX][newY].erase(it);

                            break; // Exit the loop after handling the coin
                        } else if (it->first == Key) {
                            // Handle key collection
                            movePos.erase(it); // Remove the key from the map
                            room->is_key_collected = true; // Set the key collected flag to true

                            break; // Exit the loop after handling the key
                        }
                    }
                }
                // Add checks for walls and enemies here
                else if (movePos.size() > 0 && movePos[0].first == WALL) {
                    // Handle wall collision
                } else if (movePos.size() > 0 && (movePos[0].first == ENEMY1 || movePos[0].first == ENEMY2 || movePos[0].first == BIG_ENEMY)) {
                    // Handle enemy collision
                }
            }
            break; // Exit the loop after handling the key press
        }
    }

    return coin_score; // Return the score of the coin if collected, otherwise return -1
}

HealthPlayer::~HealthPlayer() {
    UnloadSound(player_movement_sound); // Unload player movement sound
}

