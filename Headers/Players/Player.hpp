#ifndef Player_hpp
#define Player_hpp

#include <string>
#include <vector>
#include <utility> // for std::pair
#include "../Enemies/Enemy.hpp"
#include "../Rooms/Room.hpp"
#include "raylib.h"
using namespace std;

class Room;

// Abstract class for the player used as a interface for the different types of players
class Player {
    protected:
        string name;
        int health;
        int attack;
        int num_of_fights; // the number of fights the player has left, decreases by 1 each time the player fights an enemy
        int current_score;

        Sound player_movement_sound; // sound for player movement

    public:
        pair<int, int> position; // the position of the player on the map
        bool is_player_defeated = false; // to check if the player is defeated or not

        // Constructors
        Player();
        Player(string name, int health, int attack, int num_of_fights, int current_score);

        // Destructor
        virtual ~Player();

        // Setters and Getters
        void setName(string name);
        string getName() const;
        void setHealth(int health);
        int getHealth() const;
        void setAttack(int attack);
        int getAttack() const;
        void setNumOfFights(int num_of_fights);
        int getNumOfFights() const;
        void setCurrentScore(int current_score);
        int getCurrentScore() const;

        virtual int move(vector<vector<vector<pair<char, void*>>>> &map, Room* room) = 0;
        bool fight(Enemy *enemy); // the player fights an enemy
};

#endif /* Player_hpp */