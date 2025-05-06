#ifndef Room_hpp
#define Room_hpp

#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <algorithm>
#include <thread>
#include "../Enemies/Enemy.hpp"
#include "../Enemies/Enemy1.hpp"
#include "../Enemies/Enemy2.hpp"
#include "../Enemies/BigEnemy.hpp"
#include "../Players/Player.hpp"
#include "raylib.h"
using namespace std;

#define PLAYER 'P'
#define ENEMY1 '1'
#define ENEMY2 '2'
#define BIG_ENEMY 'B'
#define WALL 'W'
#define COIN 'C'
#define Special_Coin 'S'
#define Key 'K'

class Player;

class Room {
    private:
        string name;
        vector<vector<vector<pair<char, void*>>>> map; // the map of the room
        vector<Enemy*> enemies; // the enemies in the room
        int num_of_enemies; // the number of enemies in the room

        Texture2D wallTexture;
        Texture2D LivesplayerTexture;
        Texture2D HealthplayerTexture;
        Texture2D SpeedplayerTexture;
        Texture2D enemy1Texture;
        Texture2D enemy2Texture;
        Texture2D bigEnemyTexture;
        Texture2D coinTexture;
        Texture2D specialCoinTexture;
        Texture2D keyTexture;

        Sound enemy_death_sound;
        Sound player_death_sound;

    public:
        bool is_key_collected = false;
        bool is_key_placed = false; // to check if the key is placed in the room or not
        int score = 0; // the score of the player
        int Score_To_Win;

        chrono::steady_clock::time_point coinPlacedTime;
        bool coinPlaced = false;
        Enemy* trackedBigEnemy = nullptr; // Tracker for the bigenemy

        // Constructors
        Room();
        Room(string name, vector<vector<vector<pair<char, void*>>>> map, const vector<Enemy*>& enemies, int num_of_enemies, int score_to_win);
        
        // Destructor
        ~Room();

        // Setters and Getters
        void setName(string name);
        string getName() const;
        void setMap(vector<vector<vector<pair<char, void*>>>> map);
        vector<vector<vector<pair<char, void*>>>>& getMap();
        void setEnemies(vector<Enemy*> enemies);
        vector<Enemy*> getEnemies() const;
        void setNumOfEnemies(int num_of_enemies);
        int getNumOfEnemies() const;

        void printMap(Player* player); // prints the map of the room
        void movePlayer(Player* player); // moves the player in the room
        void moveEnemies(); // moves the enemies in the room
        void fightEnemy(Player* player); // the player fights an enemy
        bool checkGameStatus(); // checks if the game is over
        void show_key(vector<vector<vector<pair<char, void*>>>>& map); // shows the key in the room when the Score_To_Win is reached

        // a function to place coins in the map, will receive both normal and special coin function as function pointers
        void placeCoins(vector<vector<vector<pair<char, void*>>>>& map, int score_to_win, void (*place_normal_coin)(int, vector<vector<vector<pair<char, void*>>>>&), void (*place_special_coin)(int, vector<vector<vector<pair<char, void*>>>>&, int, int), bool is_special);
        void remove_Coin_Enemy(); // removes the coin and BigEnemy after 10 seconds
        
        template <typename... Enemies>
        void resetEnemies(Player* player, Enemies... enemies); 
};

#endif /* Room_hpp */