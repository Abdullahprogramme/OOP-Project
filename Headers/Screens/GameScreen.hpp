#ifndef GameScreen_hpp
#define GameScreen_hpp

#include "Screen.hpp"
#include "../Players/Player.hpp"
#include "../LinkedList.hpp"
#include "../Rooms/Room.hpp"
#include "raylib.h"

class GameScreen : public Screen {
    private:
        Player* player;
        LinkedList* Rooms;
        Room* currentRoom;

        Texture2D BgImage;
        
    public:
        GameScreen(Player* p);

        vector<vector<vector<pair<char, void*>>>> initializeMap(int rows, int cols);
        void processRow(const string& row, int rowIndex, vector<vector<vector<pair<char, void*>>>>& map, vector<Enemy*>& enemies, Player* player, int& enemyIndex, bool isPlayerRoom);
        void fillMapFromFile(ifstream& file, vector<vector<vector<pair<char, void*>>>>& map, vector<Enemy*>& enemies, Player* player, bool isPlayerRoom);
    
        
        ~GameScreen();

        Screen* show() override;
        void move_next_room();
        void move_previous_room();

};

#endif /* GameScreen_hpp */