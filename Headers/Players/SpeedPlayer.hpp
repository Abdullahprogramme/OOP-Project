#ifndef SpeedPlayer_hpp
#define SpeedPlayer_hpp

#include <string>
#include <vector>
#include <utility> // for std::pair
#include "Player.hpp"
#include "../Rooms/Room.hpp"
using namespace std;

class SpeedPlayer : public Player {
    public:
        SpeedPlayer();
        SpeedPlayer(string name, int health, int attack, int num_of_fights, int current_score);

        ~SpeedPlayer();

        int move(vector<vector<vector<pair<char, void*>>>> &map, Room* room) override;
};

#endif /* SpeedPlayer_hpp */