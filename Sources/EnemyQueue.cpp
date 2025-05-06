#include "../Headers/EnemyQueue.hpp"

EnemyQueue::EnemyQueue() {
    this->front = -1;
    this->rear = -1;
    this->size = 0;
}

void EnemyQueue::setFront(int front) {
    this->front = front;
}

int EnemyQueue::getFront() const {
    return this->front;
}

void EnemyQueue::setRear(int rear) {
    this->rear = rear;
}

int EnemyQueue::getRear() const {
    return this->rear;
}

void EnemyQueue::setSize(int size) {
    this->size = size;
}

int EnemyQueue::getSize() const {
    return this->size;
}

bool EnemyQueue::isEmpty() {
    return size == 0;
}

void EnemyQueue::enqueue(Enemy *enemy) {
    if (isEmpty()) {
        front = 0;
    }
    enemies[++rear] = enemy;
    size++;
}

Enemy* EnemyQueue::dequeue() {
    if (isEmpty()) {
        return nullptr;
    }
    Enemy *enemy = enemies[front];
    front++;
    size--;
    return enemy;
}

Enemy* EnemyQueue::peek() {
    if (size == 0) {
        return nullptr;
    }
    return enemies[front];
}