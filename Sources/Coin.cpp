#include "../Headers/Coins/Coin.hpp"
#include "../Headers/Enemies/BigEnemy.hpp"
#include "../Headers/Enemies/Enemy.hpp" 

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <utility> 
#include <algorithm> 

int Coin::on_screen_points = 0; 

Coin::Coin() {}

Coin::Coin(int score) {
    this->score = score;
    on_screen_points += score; // Increment the static variable when a coin is created
}

Coin::~Coin() {
    on_screen_points -= score; // Decrement the static variable when a coin is destroyed
}

void Coin::setScore(int score) {
    this->score = score;
}

int Coin::getScore() const {
    return score;
}

void Coin::place_normal_coin(int score, vector<vector<vector<pair<char, void*>>>>& map) {
    // Randomly place a normal coin in the map
    if (on_screen_points < 10) { // Limit the number of coins on screen to 10
        int row, col;
        do {
            row = rand() % map.size();
            col = rand() % map[0].size();
        } while (!map[row][col].empty() && map[row][col][0].first == 'W'); // Ensure the cell is empty and not a wall
        
        map[row][col].push_back(make_pair('C', new Coin(score))); // Place the coin in the map
    }
}

void Coin::place_special_coin(int score, vector<vector<vector<pair<char, void*>>>>& map, int score_to_win, int player_score) {
    // Check if their is already a special coin in the map
    for (int row = 0; row < map.size(); ++row) {
        for (int col = 0; col < map[row].size(); ++col) {
            for (unsigned int i = 0; i < map[row][col].size(); ++i) {
                if (map[row][col][i].first == 'S') {
                    return; // Special coin already exists, do not place another one
                }
            }
        }
    }
    
    // Randomly place a special coin in the map
    if ( (player_score % 10) > static_cast<int>(score_to_win / 2) && (player_score % 10) <= static_cast<int>(score_to_win / 2) + 1) { // Limit the number of special coins on screen to 10
        int row, col;
        do {
            row = rand() % map.size();
            col = rand() % map[0].size();
        } while (!map[row][col].empty() && map[row][col][0].first == 'W'); // Ensure the cell is empty and not a wall
        
        map[row][col].push_back(make_pair('S', new Coin(score))); // Place the special coin in the map
         
        // find a empty cell in the map near special coin and place a BigEnemy in it
        int enemy_row, enemy_col;
        do {
            enemy_row = row + (rand() % 3 - 1); // Randomly choose a row near the special coin
            enemy_col = col + (rand() % 3 - 1); // Randomly choose a column near the special coin
        } while (enemy_row < 0 || enemy_row >= map.size() || enemy_col < 0 || enemy_col >= map[0].size() || !map[enemy_row][enemy_col].empty() && map[enemy_row][enemy_col][0].first == 'W'); // Ensure the cell is empty and not a wall
        
        Enemy* enemy = new BigEnemy("Puffrage", 200, 150, "BigEnemy"); // Create a new BigEnemy object
        map[enemy_row][enemy_col].push_back(make_pair('B', enemy)); // Place the BigEnemy in the map
        enemy->position = {enemy_row, enemy_col}; // Set the position of the BigEnemy
    }
}