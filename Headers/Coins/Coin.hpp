#ifndef Coin_hpp
#define Coin_hpp

#include <vector>
#include <utility>
using namespace std;

class Coin {
    protected:
        int score; // the score of the coin

    public:
        // Constructors
        Coin();
        Coin(int score);

        static int on_screen_points; // the points on the current screen

        // Destructor
        ~Coin();

        // Setters and Getters
        void setScore(int score);
        int getScore() const;

        // two static functions to place coins on the map
        static void place_normal_coin(int score, vector<vector<vector<pair<char, void*>>>>& map);
        static void place_special_coin(int score, vector<vector<vector<pair<char, void*>>>>& map, int score_to_win, int player_score);
};

#endif /* Coin_hpp */