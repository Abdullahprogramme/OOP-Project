#include "../Headers/Rooms/Room.hpp"
#include "../Headers/Players/Player.hpp"
#include "../Headers/Players/SpeedPlayer.hpp"
#include "../Headers/Players/HealthPlayer.hpp"
#include "../Headers/Players/LivesPlayer.hpp"
#include "../Headers/Enemies/Enemy.hpp"
#include "../Headers/Enemies/Enemy1.hpp"
#include "../Headers/Enemies/Enemy2.hpp"
#include "../Headers/Enemies/BigEnemy.hpp"
#include "../Headers/EnemyQueue.hpp"
#include "../Headers/Coins/Coin.hpp"

#include "raylib.h"
#include <iostream>
#include <algorithm>
#include <thread> 
#include <chrono> 
#include <cstdlib> 
#include <cmath>  

#define PLAYER 'P'
#define ENEMY1 '1'
#define ENEMY2 '2'
#define BIG_ENEMY 'B'
#define WALL 'W'
#define COIN 'C'
#define Special_Coin 'S'
#define Key 'K'

using namespace std;

Room::Room() : name(""), map(), enemies(), num_of_enemies(0), score(0) {}

Room::Room(string name, vector<vector<vector<pair<char, void*>>>> map, const vector<Enemy*>& enemies, int num_of_enemies, int score_to_win)
    : name(name), map(map), enemies(enemies), num_of_enemies(num_of_enemies), score(0), Score_To_Win(score_to_win) {

    wallTexture = LoadTexture("Resources\\wall.png");
    LivesplayerTexture = LoadTexture("Resources\\LivesPlayer.png");
    HealthplayerTexture = LoadTexture("Resources\\HealthPlayer.png");
    SpeedplayerTexture = LoadTexture("Resources\\SpeedPlayer.png");
    enemy1Texture = LoadTexture("Resources\\Enemy1.png");
    enemy2Texture = LoadTexture("Resources\\Enemy2.png");
    bigEnemyTexture = LoadTexture("Resources\\BigEnemy.png");
    coinTexture = LoadTexture("Resources\\Coin.png");
    specialCoinTexture = LoadTexture("Resources\\Coin2.png");
    keyTexture = LoadTexture("Resources\\Key.png");

    enemy_death_sound = LoadSound("Resources\\enemy_death.mp3");
    player_death_sound = LoadSound("Resources\\player_death.mp3");
    SetSoundVolume(enemy_death_sound, 0.5f); // Set volume for enemy death sound
}

Room::~Room() {
    for (auto enemy : enemies) {
        delete enemy;
    }

    UnloadTexture(wallTexture);
    UnloadTexture(LivesplayerTexture);
    UnloadTexture(HealthplayerTexture);
    UnloadTexture(SpeedplayerTexture);
    UnloadTexture(enemy1Texture);
    UnloadTexture(enemy2Texture);
    UnloadTexture(bigEnemyTexture);
    UnloadTexture(coinTexture);
    UnloadTexture(specialCoinTexture);
    UnloadTexture(keyTexture);

    UnloadSound(enemy_death_sound);
    UnloadSound(player_death_sound);
}

void Room::setName(string name) {
    this->name = name;
}

string Room::getName() const {
    return name;
}

void Room::setMap(vector<vector<vector<pair<char, void*>>>> map) {
    this->map = map;
}

vector<vector<vector<pair<char, void*>>>>& Room::getMap() {
    return map;
}

void Room::setEnemies(vector<Enemy*> enemies) {
    this->enemies = enemies;
}

vector<Enemy*> Room::getEnemies() const {
    return enemies;
}

void Room::setNumOfEnemies(int num_of_enemies) {
    this->num_of_enemies = num_of_enemies;
}

int Room::getNumOfEnemies() const {
    return num_of_enemies;
}

void Room::printMap(Player* player) {
    const int TOP_ROW_HEIGHT = 40; // Fixed height for the top row
    const int GRID_COLS = 32;       // Number of columns in the grid
    const int GRID_ROWS = 17;       // Number of rows in the grid
    const int CELL_SIZE = 40;       // Size of each cell in the grid

    // Draw the top row for Room name, current score, and score needed to win
    DrawRectangle(0, 0, 1280, TOP_ROW_HEIGHT, LIGHTGRAY);

    // Set the y position for all text
    int textY = 20;

    // Draw the Room name, Score, Score to Win, and Player's Remaining Lives on the same row
    DrawText(("Room: " + name).c_str(), 10, textY, 20, BLACK); // Room name
    DrawText(("Score: " + to_string(score)).c_str(), 300, textY, 20, BLACK); // Current score
    DrawText(("Score to Win: " + to_string(Score_To_Win)).c_str(), 600, textY, 20, BLACK); // Score to win
    DrawText(("Fights: " + to_string(player->getNumOfFights())).c_str(), 900, textY, 20, BLACK); // Player's remaining lives

    float scale = 0.07f; // Scale for the player textures
    // Draw the map grid
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            int x = col * CELL_SIZE;
            int y = TOP_ROW_HEIGHT + row * CELL_SIZE;

            // Get the cell data
            if (row < map.size() && col < map[row].size()) {
                auto cell = map[row][col];
                if (!cell.empty()) {
                    // char cellType = cell[0].first;
                    // void* entity = cell[0].second;
                    for (unsigned int i = 0; i < cell.size(); ++i) {
                        char cellType = cell[i].first;
                        void* entity = cell[i].second;

                        // Draw based on cell type
                        if (cellType == WALL) { // Wall
                            DrawTextureEx(wallTexture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, 0.08f, WHITE);
                        } else if (cellType == PLAYER) { // Player
                            if (SpeedPlayer* sp = dynamic_cast<SpeedPlayer*>(static_cast<Player*>(entity))) {
                                DrawTextureEx(SpeedplayerTexture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, scale, WHITE);
                            } else if (HealthPlayer* hp = dynamic_cast<HealthPlayer*>(static_cast<Player*>(entity))) {
                                DrawTextureEx(HealthplayerTexture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, scale, WHITE);
                            } else if (LivesPlayer* lp = dynamic_cast<LivesPlayer*>(static_cast<Player*>(entity))) {
                                DrawTextureEx(LivesplayerTexture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, scale, WHITE);
                            }
                        } else if (cellType == ENEMY1) { // Enemy type 1
                            DrawTextureEx(enemy1Texture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, 0.30f, WHITE);
                        } else if (cellType == ENEMY2) { // Enemy type 2
                            DrawTextureEx(enemy2Texture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, 0.07f, WHITE);
                        } else if (cellType == BIG_ENEMY) { // Big enemy
                            DrawTextureEx(bigEnemyTexture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, 0.080f, WHITE);
                        } else if (cellType == COIN) { // Coin
                            DrawTextureEx(coinTexture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, scale, WHITE);
                        } else if (cellType == Special_Coin) { // Special coin
                            DrawTextureEx(specialCoinTexture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, scale, WHITE);
                        } else if (cellType == Key) { // Key
                            DrawTextureEx(keyTexture, Vector2{static_cast<float>(x), static_cast<float>(y)}, 0.0f, scale, WHITE);
                        }
                    }
                }
            }

            // Draw grid lines
            DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
        }
    }
}

void Room::moveEnemies() {
    for (auto enemy : enemies) {
        if (!is_key_collected) {
            enemy->move(map);
            if (enemy->getType() == "BigEnemy") {
                enemy->move(map);
            }
        } else {
            enemy->Enemy::move(map); // Call the base class move if the key is collected
        }
    }
}

void Room::fightEnemy(Player* player) {
    // Check if there are enemies left, right, up, or down or if the player is in the same cell as an enemy
    EnemyQueue enemyQueue;

    // Assuming the player's position is stored in playerRow and playerCol
    int playerRow = -1, playerCol = -1;
    for (int row = 0; row < map.size(); ++row) {
        for (int col = 0; col < map[row].size(); ++col) {
            if (!map[row][col].empty() && map[row][col][0].first == PLAYER) {
                playerRow = row;
                playerCol = col;
                break;
            }
        }
        if (playerRow != -1) break;
    }

    // Check adjacent cells and enqueue enemies
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {0, 0}}; // Right, Down, Left, Up, Same cell
    for (auto [dx, dy] : directions) {
        int newRow = playerRow + dx;
        int newCol = playerCol + dy;
        if (newRow >= 0 && newRow < this->map.size() && newCol >= 0 && newCol < map[newRow].size()) {
            if (!map[newRow][newCol].empty()) {
                for (unsigned int i = 0; i < map[newRow][newCol].size(); ++i) {
                    char cellType = map[newRow][newCol][i].first;
                    if (cellType == ENEMY1 || cellType == ENEMY2 || cellType == BIG_ENEMY) {
                        Enemy* enemy = static_cast<Enemy*>(map[newRow][newCol][i].second);
                        enemyQueue.enqueue(enemy);
                    }
                }
            }
        }
    }

    // Create a small new screen on top of the existing one
    const int FIGHT_SCREEN_WIDTH = 400;
    const int FIGHT_SCREEN_HEIGHT = 200;
    const int FIGHT_SCREEN_X = (1280 - FIGHT_SCREEN_WIDTH) / 2;
    const int FIGHT_SCREEN_Y = (720 - FIGHT_SCREEN_HEIGHT) / 2;

    while (!enemyQueue.isEmpty()) {
        Enemy* currentEnemy = enemyQueue.dequeue();

        // Draw the fight screen with player and enemy details
        BeginDrawing();
        DrawRectangle(FIGHT_SCREEN_X, FIGHT_SCREEN_Y, FIGHT_SCREEN_WIDTH, FIGHT_SCREEN_HEIGHT, DARKGRAY);
        DrawText("Fight!", FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 10, 20, WHITE);
        DrawText(("Player HP: " + to_string(player->getHealth())).c_str(), FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 50, 20, WHITE);
        DrawText(("Player Attack: " + to_string(player->getAttack())).c_str(), FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 80, 20, WHITE);
        DrawText(("Enemy HP: " + to_string(currentEnemy->getHealth())).c_str(), FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 110, 20, WHITE);
        DrawText(("Enemy Attack: " + to_string(currentEnemy->getAttack())).c_str(), FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 140, 20, WHITE);
        EndDrawing();

        // Pause for 2 seconds before starting the fight
        this_thread::sleep_for(chrono::seconds(2));

        // Simulate the fight
        bool is_player_defeated = false;
        if (currentEnemy->getHealth() > 0 && player->getHealth() > 0) {
            if (player->getNumOfFights() > 0) {
                is_player_defeated = player->fight(currentEnemy);
            } else {
                player->is_player_defeated = true; // Player is defeated
            }
        }

        // Update the fight screen with the result
        BeginDrawing();
        DrawRectangle(FIGHT_SCREEN_X, FIGHT_SCREEN_Y, FIGHT_SCREEN_WIDTH, FIGHT_SCREEN_HEIGHT, DARKGRAY);
        DrawText("Fight!", FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 10, 20, WHITE);
        DrawText(("Player HP: " + to_string(player->getHealth())).c_str(), FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 50, 20, WHITE);
        DrawText(("Player Attack: " + to_string(player->getAttack())).c_str(), FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 80, 20, WHITE);
        DrawText(("Enemy HP: " + to_string(currentEnemy->getHealth())).c_str(), FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 110, 20, WHITE);
        DrawText(("Enemy Attack: " + to_string(currentEnemy->getAttack())).c_str(), FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 140, 20, WHITE);

        // Display the result of the fight
        if (is_player_defeated) {
            PlaySound(player_death_sound); // Play player death sound
            DrawText("Player was defeated!", FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 170, 20, RED);
        } else {
            PlaySound(enemy_death_sound); // Play enemy death sound
            DrawText((currentEnemy->getName() + " was defeated!").c_str(), FIGHT_SCREEN_X + 10, FIGHT_SCREEN_Y + 170, 20, GREEN);
        }
        EndDrawing();

        // Pause for 2 seconds to display the result
        this_thread::sleep_for(chrono::seconds(2));

        // If the enemy was defeated, remove it from the map and enemies vector
        if (!is_player_defeated) {
            for (int row = 0; row < map.size(); ++row) {
                for (int col = 0; col < map[row].size(); ++col) {
                    for (unsigned int i = 0; i < map[row][col].size(); ++i) {
                        if (map[row][col][i].first == ENEMY1 || map[row][col][i].first == ENEMY2 || map[row][col][i].first == BIG_ENEMY) {
                            if (map[row][col][i].second == currentEnemy) {
                                map[row][col].erase(map[row][col].begin() + i); // Remove the enemy from the map
                                break;
                            }
                        }
                    }
                }
            }
            // Remove the enemy from the enemies vector
            auto it = std::remove_if(enemies.begin(), enemies.end(), [currentEnemy](Enemy* enemy) { return enemy == currentEnemy; });
            enemies.erase(it, enemies.end()); // Remove the enemy from the vector

            delete currentEnemy; // Free memory
        } else {
            vector<Enemy*> remainingEnemies;
            while (!enemyQueue.isEmpty()) {
                remainingEnemies.push_back(enemyQueue.dequeue());
            }
            remainingEnemies.push_back(currentEnemy); // Add the current enemy back to the remaining enemies

            // Reset the current enemy and all remaining enemies
            if (remainingEnemies.size() == 1) {
                resetEnemies(player, remainingEnemies[0]);
            } else if (remainingEnemies.size() == 2) {
                resetEnemies(player, remainingEnemies[0], remainingEnemies[1]);
            } else if (remainingEnemies.size() == 3) {
                resetEnemies(player, remainingEnemies[0], remainingEnemies[1], remainingEnemies[2]);
            } else if (remainingEnemies.size() == 4) {
                resetEnemies(player, remainingEnemies[0], remainingEnemies[1], remainingEnemies[2], remainingEnemies[3]);
            }
            break;
        }
    }
}

bool Room::checkGameStatus() {
    if (score >= Score_To_Win && is_key_collected) {
        return true; 
    } else if (score >= Score_To_Win && !is_key_collected) {
        // Show the key in the room
        show_key(map);
        return false; // Game is not over yet, but key is shown
    } else return false; // Game is not over yet 
}

void Room::movePlayer(Player* player) {
    // Assuming player is a pointer to the player object
    if (player) {
        int coin_score = player->move(map, this); // Move the player in the room
        (coin_score != -1) ? score += coin_score : score; // Update the score if a coin is collected
    }
}

void Room::show_key(vector<vector<vector<pair<char, void*>>>>& map) {
    if (score >= Score_To_Win && !is_key_placed) {
        // randomly place the key in the map and first remove all the coins from the map
        for (int row = 0; row < map.size(); ++row) {
            for (int col = 0; col < map[row].size(); ++col) {
                for (unsigned int i = 0; i < map[row][col].size(); ++i) {
                    if (map[row][col][i].first == COIN || map[row][col][i].first == Special_Coin) {
                        // Remove the coin from the map and call their destructor
                        Coin* coin = static_cast<Coin*>(map[row][col][i].second);
                        delete coin; // Free memory
                        map[row][col].erase(map[row][col].begin() + i); // Remove the coin from the map
                        break;
                    }
                }
            }
        }

        // Randomly place the key in the map which is a valid cell and not a wall and not occupied by an enemy
        int row, col;
        do {
            row = GetRandomValue(0, map.size() - 1);
            col = GetRandomValue(0, map[0].size() - 1);

            bool is_valid_cell = true;
            for (unsigned int i = 0; i < map[row][col].size(); ++i) {
                if (map[row][col][i].first == WALL || map[row][col][i].first == ENEMY1 || map[row][col][i].first == ENEMY2 || map[row][col][i].first == BIG_ENEMY) {
                    is_valid_cell = false;
                    break;
                }
            }

            if (is_valid_cell) {
                // Place the key in the cell
                map[row][col].push_back(make_pair(Key, nullptr)); // Assuming the key doesn't need any specific data
                is_key_placed = true; // Set the key placed flag to true
                break;
            }
        } while (true);
    }
}

void Room::placeCoins(vector<vector<vector<pair<char, void*>>>>& map, int score_to_win, void (*place_normal_coin)(int, vector<vector<vector<pair<char, void*>>>>&), void (*place_special_coin)(int, vector<vector<vector<pair<char, void*>>>>&, int, int), bool is_special) {
    if (!is_key_placed) {
        if (is_special) {
            place_special_coin(3, map, score_to_win, score); // place special coin
        } else {
            place_normal_coin(1, map); // place normal coin
        }
    }
}

void Room::remove_Coin_Enemy() {
    static const int ROWS = 17, COLS = 32;

    if (!coinPlaced) {
        for (int i = 0; i < ROWS && !coinPlaced; ++i) {
            for (int j = 0; j < COLS && !coinPlaced; ++j) {
                auto& cell = map[i][j];
                auto coinIt = std::find_if(cell.begin(), cell.end(), [](const pair<char, void*>& p) {
                    return p.first == Special_Coin;
                });

                if (coinIt != cell.end()) {
                    coinPlacedTime = chrono::steady_clock::now();
                    coinPlaced = true;

                    // Find BigEnemy just once
                    for (int row = 0; row < ROWS && !trackedBigEnemy; ++row) {
                        for (int col = 0; col < COLS && !trackedBigEnemy; ++col) {
                            auto& enemyCell = map[row][col];
                            auto enemyIt = std::find_if(enemyCell.begin(), enemyCell.end(), [](const pair<char, void*>& p) {
                                return p.first == BIG_ENEMY;
                            });

                            if (enemyIt != enemyCell.end()) {
                                trackedBigEnemy = static_cast<Enemy*>(enemyIt->second);
                                enemies.push_back(trackedBigEnemy);
                                num_of_enemies++;
                            }
                        }
                    }
                }
            }
        }
    }

    // Check if 10 seconds have passed since coin was placed
    if (coinPlaced && chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - coinPlacedTime).count() >= 10) {
        // Remove the special coin and the big enemy
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                auto& cell = map[i][j];

                // Remove Special Coin if found
                auto coinIt = std::find_if(cell.begin(), cell.end(), [](const pair<char, void*>& p) {
                    return p.first == Special_Coin;
                });
                if (coinIt != cell.end()) {
                    cell.erase(coinIt);
                }

                // Remove Tracked Big Enemy if found
                if (trackedBigEnemy) {
                    auto enemyIt = std::find_if(cell.begin(), cell.end(), [this](const pair<char, void*>& p) {
                        return p.first == BIG_ENEMY && p.second == trackedBigEnemy;
                    });
                    if (enemyIt != cell.end()) {
                        cell.erase(enemyIt);
                    }
                }
            }
        }

        // Remove from enemies vector
        if (trackedBigEnemy) {
            auto it = std::remove(enemies.begin(), enemies.end(), trackedBigEnemy);
            enemies.erase(it, enemies.end());
            delete trackedBigEnemy;
            trackedBigEnemy = nullptr;
            num_of_enemies--;
        }

        coinPlaced = false;
    }
}


template <typename... Enemies>
void Room::resetEnemies(Player* player, Enemies... enemies) {
    // Helper lambda to reset a single enemy
    auto resetEnemy = [this, player](Enemy* enemy) {
        int newRow, newCol;

        // Generate a new position for the enemy
        do {
            newRow = rand() % map.size();       // Random row within map bounds
            newCol = rand() % map[0].size();   // Random column within map bounds
        } while ((abs(newRow - player->position.first) <= 2 && abs(newCol - player->position.second) <= 2) || // Ensure enemy is not too close to the player
                 !map[newRow][newCol].empty()); // Ensure the cell is empty

        // Remove the enemy from its current position on the map
        for (int row = 0; row < map.size(); ++row) {
            for (int col = 0; col < map[row].size(); ++col) {
                for (unsigned int j = 0; j < map[row][col].size(); ++j) {
                    if (map[row][col][j].second == enemy) {
                        map[row][col].erase(map[row][col].begin() + j); // Remove the enemy from the map
                        break;
                    }
                }
            }
        }

        char enemyType = (enemy->getType() == "Enemy1") ? ENEMY1 : (enemy->getType() == "Enemy2") ? ENEMY2 : BIG_ENEMY; // Determine enemy type

        // Place the enemy at the new position
        map[newRow][newCol].push_back({enemyType, enemy}); // Place the enemy on the map
        enemy->position = {newRow, newCol};                // Update the enemy's position
    };

    // Expand the parameter pack and reset each enemy
    (resetEnemy(enemies), ...);
}
