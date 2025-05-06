#ifndef LivesPlayer_hpp
#define LivesPlayer_hpp

#include <string>
#include <vector>
#include <utility>
#include "Player.hpp"
#include "../Rooms/Room.hpp"
using namespace std;

class LivesPlayer : public Player {
    public:
        LivesPlayer();
        LivesPlayer(string name, int health, int attack, int num_of_fights, int current_score);

        ~LivesPlayer();

        int move(vector<vector<vector<pair<char, void*>>>> &map, Room* room) override;
};

#endif /* LivesPlayer_hpp */