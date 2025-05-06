#include "../Headers/Screens/GameScreen.hpp"
#include "../Headers/Players/SpeedPlayer.hpp"
#include "../Headers/Players/HealthPlayer.hpp"
#include "../Headers/Players/LivesPlayer.hpp"
#include "../Headers/Rooms/Room.hpp"
#include "../Headers/LinkedList.hpp"
#include "../Headers/Coins/Coin.hpp"
#include "../Headers/Screens/IntroScreen.hpp"
#include "../Headers/Enemies/Enemy.hpp"
#include "../Headers/Enemies/Enemy1.hpp"
#include "../Headers/Enemies/Enemy2.hpp"
#include "../raylib.h"

#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <utility> 
#include <algorithm> 
#include <chrono> 

GameScreen::GameScreen(Player* p) : player(p) {
    BgImage = LoadTexture("Resources\\Dungeon.png");

    // Initialize maps
    auto map1 = initializeMap(17, 32);
    auto map2 = initializeMap(17, 32);
    auto map3 = initializeMap(17, 32);

    // Create enemies
    vector<Enemy*> enemies1 = {
        new Enemy1("Hornox", 50, 50, "Enemy 1"),
        new Enemy1("Hornox", 50, 50, "Enemy 1")
    };

    vector<Enemy*> enemies2 = {
        new Enemy1("Hornox", 50, 50, "Enemy 1"),
        new Enemy1("Hornox", 50, 50, "Enemy 1"),
        new Enemy2("Cryobot", 100, 100, "Enemy 2")
    };

    vector<Enemy*> enemies3 = {
        new Enemy1("Hornox", 50, 50, "Enemy 1"),
        new Enemy1("Hornox", 50, 50, "Enemy 1"),
        new Enemy2("Cryobot", 100, 100, "Enemy 2"),
        new Enemy2("Cryobot", 100, 100, "Enemy 2")
    };

    ifstream file1("Resources\\Map1.txt"), file2("Resources\\Map2.txt"), file3("Resources\\Map3.txt");
    if (!file1 || !file2 || !file3) {
        throw runtime_error("Error opening map files.");
    }

    // Fill maps
    fillMapFromFile(file1, map1, enemies1, player, true);
    fillMapFromFile(file2, map2, enemies2, player, false);
    fillMapFromFile(file3, map3, enemies3, player, false);

    file1.close(); file2.close(); file3.close();

    // Create rooms
    Room* room1 = new Room("Mystic Cavern", map1, enemies1, 2, 10);
    Room* room2 = new Room("Celestial Spire", map2, enemies2, 3, 10);
    Room* room3 = new Room("Shadow Hall", map3, enemies3, 4, 10);

    // Add to linked list
    Rooms = new LinkedList();
    try {
        Rooms->add(room1);
        Rooms->add(room2);
        Rooms->add(room3);
    } catch (const exception& e) {
        cerr << "Error adding rooms to linked list: " << e.what() << endl;
    }
}


// Helper function to initialize a map with given dimensions
vector<vector<vector<pair<char, void*>>>> GameScreen::initializeMap(int rows, int cols) {
    return vector<vector<vector<pair<char, void*>>>>(rows, vector<vector<pair<char, void*>>>(cols));
}

// Helper function to process a single row of the map
void GameScreen::processRow(const string& row, int rowIndex, vector<vector<vector<pair<char, void*>>>>& map, vector<Enemy*>& enemies, Player* player, int& enemyIndex, bool isPlayerRoom) {
    for (int j = 0; j < row.length(); j++) {
        char cell = row[j];
        if (cell == '#') {
            map[rowIndex][j].push_back({'W', nullptr});
        } else if (cell == 'P') {
            map[rowIndex][j].push_back({'P', isPlayerRoom ? player : nullptr});
            if (isPlayerRoom) player->position = {rowIndex, j};
        } else if (cell == '1' || cell == '2') {
            if (enemyIndex < enemies.size()) {
                map[rowIndex][j].push_back({cell, enemies[enemyIndex]});
                enemies[enemyIndex]->position = {rowIndex, j};
                enemyIndex++;
            }
        }
    }
}

// Helper function to read and fill the map from file
void GameScreen::fillMapFromFile(ifstream& file, vector<vector<vector<pair<char, void*>>>>& map, vector<Enemy*>& enemies, Player* player, bool isPlayerRoom) {
    string row;
    int enemyIndex = 0;
    for (int i = 0; i < map.size(); i++) {
        getline(file, row);
        processRow(row, i, map, enemies, player, enemyIndex, isPlayerRoom);
    }
}


GameScreen::~GameScreen() {
    delete player;  // Free player memory
    UnloadTexture(BgImage);  // Unload the background image

    delete Rooms;   // Free rooms linked list memory
}

Screen* GameScreen::show() {

    // Set the current room to the first room in the linked list
    currentRoom = Rooms->getHead()->data;

    const float enemyMoveInterval = 1.0f; // Time interval for enemy movement (in seconds)
    float enemyMoveTimer = 0.0f; // Timer to track enemy movement

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        // Draw the background image
        DrawTextureEx(BgImage, {0, 0}, 0.0f, (float)GetScreenWidth() / BgImage.width, WHITE);

        // Print the map of the current room
        currentRoom->printMap(player);

        if (!player->is_player_defeated && !currentRoom->checkGameStatus()) {

            // Placing a coin, the function itself will check if the coin is to be placed or not
            currentRoom->placeCoins(currentRoom->getMap(), currentRoom->Score_To_Win, Coin::place_normal_coin, Coin::place_special_coin, true); // Place special coin
            currentRoom->placeCoins(currentRoom->getMap(), currentRoom->Score_To_Win, Coin::place_normal_coin, Coin::place_special_coin, false); // Place normal coin

            // Remove the coin and BigEnemy after 10 seconds
            currentRoom->remove_Coin_Enemy(); // Call the function to remove the coin and BigEnemy

            // First move player, then move enemies
            currentRoom->movePlayer(player);

            if (enemyMoveTimer >= enemyMoveInterval) {
                currentRoom->moveEnemies();  // Move enemies every second
                enemyMoveTimer = 0.0f;  // Reset the timer
            } else {
                enemyMoveTimer += GetFrameTime();  // Increment the timer
            }

            // After this, check if the player fights the enemies
            currentRoom->fightEnemy(player);

        }

        // Check the game status
        if (player->is_player_defeated || currentRoom->checkGameStatus()) {
            // Draw the box
            const int BOX_WIDTH = 600;
            const int BOX_HEIGHT = 300;
            const int BOX_X = (GetScreenWidth() - BOX_WIDTH) / 2;
            const int BOX_Y = (GetScreenHeight() - BOX_HEIGHT) / 2;
            DrawRectangle(BOX_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, DARKGRAY);

            // Draw the "Yes" and "No" buttons
            const int BUTTON_WIDTH = 150;
            const int BUTTON_HEIGHT = 60;
            const int BUTTON_Y = BOX_Y + 200;
            const int YES_BUTTON_X = BOX_X + 100;
            const int NO_BUTTON_X = BOX_X + 350;

            // If the player is dead, show the game over screen
            if (player->is_player_defeated) {
                DrawText("Game Over!", BOX_X + 20, BOX_Y + 30, 30, WHITE);
                DrawText("You have been defeated.", BOX_X + 20, BOX_Y + 80, 20, WHITE);
                DrawText("Do you want to play again?", BOX_X + 20, BOX_Y + 120, 20, WHITE);

                // Draw the "Yes" button
                DrawRectangle(YES_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, GREEN);
                DrawText("Yes", YES_BUTTON_X + 50, BUTTON_Y + 20, 20, WHITE);

                // Draw the "No" button
                DrawRectangle(NO_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, RED);
                DrawText("No", NO_BUTTON_X + 55, BUTTON_Y + 20, 20, WHITE);

                // Check for button clicks
                Vector2 mousePosition = GetMousePosition();
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Check if "Yes" button is clicked
                    if (mousePosition.x >= YES_BUTTON_X && mousePosition.x <= YES_BUTTON_X + BUTTON_WIDTH &&
                        mousePosition.y >= BUTTON_Y && mousePosition.y <= BUTTON_Y + BUTTON_HEIGHT) {
                        return new IntroScreen();  // Restart the game
                    }

                    // Check if "No" button is clicked
                    if (mousePosition.x >= NO_BUTTON_X && mousePosition.x <= NO_BUTTON_X + BUTTON_WIDTH &&
                        mousePosition.y >= BUTTON_Y && mousePosition.y <= BUTTON_Y + BUTTON_HEIGHT) {
                        return nullptr;  // Exit the game
                    }
                }
            }
            // If the key is collected and it's the last room, end the game
            else if (Rooms->isLast() && currentRoom->is_key_collected) {
                // Draw the text inside the box
                DrawText(("Congratulations " + player->getName() + "! You have completed the game!").c_str(), BOX_X + 20, BOX_Y + 30, 20, WHITE);
                DrawText("Do you want to play again?", BOX_X + 20, BOX_Y + 80, 20, WHITE);

                // Draw the "Yes" button
                DrawRectangle(YES_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, GREEN);
                DrawText("Yes", YES_BUTTON_X + 50, BUTTON_Y + 20, 20, WHITE);

                // Draw the "No" button
                DrawRectangle(NO_BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, RED);
                DrawText("No", NO_BUTTON_X + 55, BUTTON_Y + 20, 20, WHITE);

                // Check for button clicks
                Vector2 mousePosition = GetMousePosition();
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Check if "Yes" button is clicked
                    if (mousePosition.x >= YES_BUTTON_X && mousePosition.x <= YES_BUTTON_X + BUTTON_WIDTH &&
                        mousePosition.y >= BUTTON_Y && mousePosition.y <= BUTTON_Y + BUTTON_HEIGHT) {
                        return new IntroScreen();  // Restart the game
                    }

                    // Check if "No" button is clicked
                    if (mousePosition.x >= NO_BUTTON_X && mousePosition.x <= NO_BUTTON_X + BUTTON_WIDTH &&
                        mousePosition.y >= BUTTON_Y && mousePosition.y <= BUTTON_Y + BUTTON_HEIGHT) {
                        return nullptr;  // Exit the game
                    }
                }
            } else {
                // Move to the next room if the key is collected
                move_next_room();
            }
        }

        EndDrawing();
    }

    CloseWindow();  // Close the game window
    return nullptr;  // Return to the previous screen
}

void GameScreen::move_next_room() {
    if (currentRoom->is_key_collected == true) {
        Rooms->next();
        currentRoom = Rooms->getCurrent()->data;
        // resetting the player's position to the new room
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 32; j++) {
                if (currentRoom->getMap()[i][j].size() > 0 && currentRoom->getMap()[i][j][0].first == 'P') {
                    player->position = {i, j};  // Set player position
                    currentRoom->getMap()[i][j][0].second = player;  // Set player in the new room
                }
            }
        }

    }
}

void GameScreen::move_previous_room() {
    Rooms->prev();
    currentRoom = Rooms->getCurrent()->data;
}