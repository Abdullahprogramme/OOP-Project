#ifndef Enemy1_hpp
#define Enemy1_hpp

#include <string>
#include <vector>
#include <utility>
#include "Enemy.hpp"
using namespace std;

class Enemy1 : public Enemy {
    public:
        Enemy1();
        Enemy1(string name, int health, int attack, string type);

        ~Enemy1();

        void move(vector<vector<vector<pair<char, void*>>>> &map) override;
        void findPlayer(vector<vector<char>> &map, int player_x, int player_y) override;
};

#endif /* Enemy1_hpp */