#include "../Headers/Enemies/Enemy2.hpp"

#include <algorithm>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
#include <iostream>
using namespace std;

#define PLAYER 'P'
#define ENEMY1 '1'
#define ENEMY2 '2'
#define BIG_ENEMY 'B'
#define WALL 'W'
#define COIN 'C'

Enemy2::Enemy2() : Enemy("Enemy1", 100, 100, "Enemy2") {}

Enemy2::Enemy2(string name, int health, int attack, string type) : Enemy(name, health, attack, type = "Enemy2") {}

Enemy2::~Enemy2() {}

void Enemy2::move(vector<vector<vector<pair<char, void*>>>> &map) {
    int rows = map.size();
    int cols = map[0].size();

    vector<vector<char>> simplifiedMap(rows, vector<char>(cols, '.'));
    int player_x = -1, player_y = -1;

    // Fill the simplified map and find player position
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!map[i][j].empty()) {
                char symbol = map[i][j][0].first;
                switch (symbol) {
                    case WALL: simplifiedMap[i][j] = '#'; break;
                    case ENEMY1: simplifiedMap[i][j] = '1'; break;
                    case ENEMY2: simplifiedMap[i][j] = '2'; break;
                    case BIG_ENEMY: simplifiedMap[i][j] = 'B'; break;
                    case PLAYER:
                        simplifiedMap[i][j] = 'P';
                        player_x = i;
                        player_y = j;
                        break;
                }
            }
        }
    }

    if (player_x == -1 || player_y == -1) {
        cerr << "Error: Player not found on the map." << endl;
        return;
    }

    // Remove the enemy's old position marker from the map
    auto &oldCell = map[position.first][position.second];
    oldCell.erase(
        remove_if(oldCell.begin(), oldCell.end(),
                  [this](const pair<char, void*> &p) { 
                      return (p.first == ENEMY1 || p.first == ENEMY2 || p.first == BIG_ENEMY) && p.second == this; 
                  }),
        oldCell.end());

    // Call findPlayer to determine the next move
    findPlayer(simplifiedMap, player_x, player_y);

    char enemyType = (type == "Enemy1") ? ENEMY1 : (type == "Enemy2") ? ENEMY2 : BIG_ENEMY;

    // Update the enemy's position in the map
    auto &newCell = map[position.first][position.second];
    newCell.push_back(make_pair(enemyType, this));
}

void Enemy2::findPlayer(vector<vector<char>> &map, int player_x, int player_y) {
    int rows = map.size();
    int cols = map[0].size();

    // Dijkstra's Algorithm setup
    vector<vector<int>> dist(rows, vector<int>(cols, numeric_limits<int>::max()));
    vector<vector<pair<int, int>>> prev(rows, vector<pair<int, int>>(cols, {-1, -1}));
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> pq;

    // Start from the enemy's position
    int start_x = position.first;
    int start_y = position.second;
    dist[start_x][start_y] = 0;
    pq.push({0, {start_x, start_y}});

    // Directions for movement (up, down, left, right)
    vector<pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    // Dijkstra's Algorithm
    while (!pq.empty()) {
        auto [current_dist, current_pos] = pq.top();
        pq.pop();

        int x = current_pos.first;
        int y = current_pos.second;

        if (current_dist > dist[x][y]) continue;

        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && map[nx][ny] != '#') {
                int new_dist = dist[x][y] + 1;
                if (new_dist < dist[nx][ny]) {
                    dist[nx][ny] = new_dist;
                    prev[nx][ny] = {x, y};
                    pq.push({new_dist, {nx, ny}});
                }
            }
        }
    }

    // Trace back the path from the player to the enemy
    pair<int, int> target = {player_x, player_y};
    pair<int, int> nextMove = {-1, -1};

    while (target != make_pair(start_x, start_y)) {
        if (prev[target.first][target.second] == make_pair(start_x, start_y)) {
            nextMove = target;
            break;
        }
        target = prev[target.first][target.second];
    }

    // Introduce randomness to simulate errors
    if (nextMove != make_pair(-1, -1)) {
        // 30% chance to make a random move instead of the optimal one
        if (rand() % 100 < 30) {
            for (auto [dx, dy] : directions) {
                int randomX = position.first + dx;
                int randomY = position.second + dy;

                if (randomX >= 0 && randomX < rows && randomY >= 0 && randomY < cols && map[randomX][randomY] != '#') {
                    position = {randomX, randomY};
                    return;
                }
            }
        }

        // Otherwise, follow the optimal path
        position = nextMove;
    }
}