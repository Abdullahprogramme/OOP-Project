#ifndef HealthPlayer_hpp
#define HealthPlayer_hpp

#include <string>
#include <vector>
#include <utility>
#include "Player.hpp"
#include "../Rooms/Room.hpp"
using namespace std;

class HealthPlayer : public Player {
    public:
        HealthPlayer();
        HealthPlayer(string name, int health, int attack, int num_of_fights, int current_score);

        ~HealthPlayer();

        int move(vector<vector<vector<pair<char, void*>>>> &map, Room* room) override;
};

#endif /* HealthPlayer_hpp */