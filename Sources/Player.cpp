#include "../Headers/Players/Player.hpp"
#include <string>

Player::Player() {}

Player::Player(string name = "Player", int health = 100, int attack = 10, int num_of_fights = 3, int current_score = 0) {
    this->name = name;
    this->health = health;
    this->attack = attack;
    this->num_of_fights = num_of_fights;
    this->current_score = current_score;
}

Player::~Player() {}

void Player::setName(string name) {
    this->name = name;
}

string Player::getName() const {
    return this->name;
}

void Player::setHealth(int health) {
    this->health = health;
}

int Player::getHealth() const {
    return this->health;
}

void Player::setAttack(int attack) {
    this->attack = attack;
}

int Player::getAttack() const {
    return this->attack;
}

void Player::setNumOfFights(int num_of_fights) {
    this->num_of_fights = num_of_fights;
}

int Player::getNumOfFights() const {
    return this->num_of_fights;
}

void Player::setCurrentScore(int current_score) {
    this->current_score = current_score;
}

int Player::getCurrentScore() const {
    return this->current_score;
}

bool Player::fight(Enemy *enemy) {
    // check if the player has enough fights left
    if (num_of_fights > 0) {

        // Player attacks the enemy
        enemy->setHealth(enemy->getHealth() - attack);
        
        // Decrease the number of fights left
        num_of_fights--;

        // check if the player is defeated or not
        if (health - enemy->getAttack() <= 0 && enemy->getHealth() > 0) {
            return true; // Player is defeated
        } else if (health - enemy->getAttack() > 0 && enemy->getHealth() <= 0) {
            // Enemy is defeated
            return false; // Player is not defeated
        } else if (health - enemy->getAttack() <= 0 && enemy->getHealth() <= 0) {
            // Both player and enemy are defeated
            return true; // Player is defeated
        } else {
            return false; // Player is not defeated
        }
    } else {
        return false; // No fights left
    }
}