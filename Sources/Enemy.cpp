#include "../Headers/Enemies/Enemy.hpp"

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


Enemy::Enemy() {
    this->name = "";
    this->health = 0;
    this->attack = 0;
    this->type = "";
}

Enemy::Enemy(string name, int health, int attack, string type) {
    this->name = name;
    this->health = health;
    this->attack = attack;
    this->type = type;
}

Enemy::~Enemy() {}

void Enemy::setName(string name) {
    this->name = name;
}

string Enemy::getName() const {
    return this->name;
}

void Enemy::setHealth(int health) {
    this->health = health;
}

int Enemy::getHealth() const {
    return this->health;
}

void Enemy::setAttack(int attack) {
    this->attack = attack;
}

int Enemy::getAttack() const {
    return this->attack;
}

void Enemy::setType(string type) {
    this->type = type;
}

string Enemy::getType() const {
    return this->type;
}

void Enemy::move(vector<vector<vector<pair<char, void*>>>> &map) {
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

    // Call findPlayer to determine the next move
    findPlayer(simplifiedMap, player_x, player_y);

    // Determine enemy type char
    char enemyType = (type == "Enemy1") ? ENEMY1 : (type == "Enemy2") ? ENEMY2 : BIG_ENEMY;

    // Push enemy into new position
    map[position.first][position.second].emplace_back(enemyType, this);
}

void Enemy::findPlayer(vector<vector<char>> &map, int player_x, int player_y) {
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

    // Update the enemy's position to the next move
    if (nextMove != make_pair(-1, -1)) {
        position = nextMove;
    }
}