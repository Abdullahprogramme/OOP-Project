#ifndef BigEnemy_hpp
#define BigEnemy_hpp

#include <string>
#include <vector>
#include <utility>
#include "Enemy.hpp"
using namespace std;

class BigEnemy : public Enemy {
    public:
        BigEnemy();
        BigEnemy(string name, int health, int attack, string type);

        ~BigEnemy();

        void move(vector<vector<vector<pair<char, void*>>>> &map) override;
        void findPlayer(vector<vector<char>> &map, int player_x, int player_y) override;
};

#endif /* BigEnemy_hpp */