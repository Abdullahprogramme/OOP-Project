#ifndef Enemy2_hpp
#define Enemy2_hpp

#include <string>
#include <vector>
#include <utility>  
#include "Enemy.hpp"
using namespace std;

class Enemy2 : public Enemy {
    public:
        Enemy2();
        Enemy2(string name, int health, int attack, string type);

        ~Enemy2();

        void move(vector<vector<vector<pair<char, void*>>>> &map) override;
        void findPlayer(vector<vector<char>> &map, int player_x, int player_y) override;
};

#endif /* Enemy2_hpp */