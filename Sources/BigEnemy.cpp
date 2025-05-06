#include "../Headers/Enemies/BigEnemy.hpp"

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

BigEnemy::BigEnemy() : Enemy("BigEnemy", 200, 150, "BigEnemy") {}

BigEnemy::BigEnemy(string name, int health, int attack, string type) : Enemy(name, health, attack, type = "BigEnemy") {}

BigEnemy::~BigEnemy() {}

void BigEnemy::move(vector<vector<vector<pair<char, void*>>>> &map) {
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

    // Remove enemy from its old position
    auto &oldCell = map[position.first][position.second];
    oldCell.erase(
        remove_if(oldCell.begin(), oldCell.end(),
                  [this](const pair<char, void*> &p) {
                      return (p.first == ENEMY1 || p.first == ENEMY2 || p.first == BIG_ENEMY) && p.second == this;
                  }),
        oldCell.end());

    // Use pathfinding to move towards player
    findPlayer(simplifiedMap, player_x, player_y);

    // Determine enemy type char
    char enemyType = (type == "Enemy1") ? ENEMY1 : (type == "Enemy2") ? ENEMY2 : BIG_ENEMY;

    // Push enemy into new position
    map[position.first][position.second].emplace_back(enemyType, this);
}

void BigEnemy::findPlayer(vector<vector<char>> &map, int player_x, int player_y) {
    int rows = map.size(), cols = map[0].size();
    vector<vector<int>> dist(rows, vector<int>(cols, INT_MAX));
    vector<vector<pair<int, int>>> prev(rows, vector<pair<int, int>>(cols, {-1, -1}));

    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> pq;

    int sx = position.first, sy = position.second;
    dist[sx][sy] = 0;
    pq.push({0, {sx, sy}});

    vector<pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    while (!pq.empty()) {
        auto [curr_dist, pos] = pq.top(); pq.pop();
        int x = pos.first, y = pos.second;

        if (curr_dist > dist[x][y]) continue;

        for (auto [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                int new_dist = curr_dist + 1;
                if (new_dist < dist[nx][ny]) {
                    dist[nx][ny] = new_dist;
                    prev[nx][ny] = {x, y};
                    pq.push({new_dist, {nx, ny}});
                }
            }
        }
    }

    // Reconstruct next step from enemy to player
    pair<int, int> target = {player_x, player_y};
    pair<int, int> nextMove = {-1, -1};

    while (target != make_pair(sx, sy)) {
        if (prev[target.first][target.second] == make_pair(sx, sy)) {
            nextMove = target;
            break;
        }
        target = prev[target.first][target.second];
    }

    if (nextMove != make_pair(-1, -1)) {
        position = nextMove;
    }
}
