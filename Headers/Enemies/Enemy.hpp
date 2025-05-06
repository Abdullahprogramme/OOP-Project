#ifndef Enemy_hpp
#define Enemy_hpp

#include <string>
#include <vector>
#include <utility>
using namespace std;

// Abstract class for the enemy used as a interface for the different types of enemies
class Enemy {
    protected:
        string name;
        int health;
        int attack;
        string type;

    public:
        pair<int, int> position;
        
        // Constructors
        Enemy();
        Enemy(string name, int health, int attack, string type);

        // Destructor
        virtual ~Enemy();

        // Setters and Getters
        void setName(string name);
        string getName() const;
        void setHealth(int health);
        int getHealth() const;
        void setAttack(int attack);
        int getAttack() const;
        void setType(string type);
        string getType() const;

        virtual void move(vector<vector<vector<pair<char, void*>>>> &map) = 0;
        virtual void findPlayer(vector<vector<char>> &map, int player_x, int player_y) = 0; // Dijkstra's Algorithm, move() will call this function
};

#endif /* Enemy_hpp */