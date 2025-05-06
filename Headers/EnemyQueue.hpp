#ifndef EnemyQueue_hpp
#define EnemyQueue_hpp

#include "../Headers/Enemies/Enemy.hpp"
#include <array>

using namespace std;

class EnemyQueue {
    private:
        array<Enemy*, 4> enemies; // the array of enemies will at most have 4 elements
        int front; // the index of the front of the queue
        int rear; // the index of the rear of the queue
        int size; // the number of elements in the queue

    public:
        // Constructor
        EnemyQueue();

        // Setters and Getters
        void setFront(int front);
        int getFront() const;
        void setRear(int rear);
        int getRear() const;
        void setSize(int size);
        int getSize() const;

        // Methods
        void enqueue(Enemy *enemy); // adds an enemy to the queue
        Enemy* dequeue(); // removes an enemy from the queue
        Enemy* peek(); // returns the enemy at the front of the queue
        bool isEmpty(); // checks if the queue is empty
};

#endif /* EnemyQueue_hpp */