#ifndef LinkedList_hpp
#define LinkedList_hpp

#include "../Headers/Rooms/Room.hpp"
using namespace std;

// Node class for the Singly Linked List
class Node {
    public:
        Room *data; // the data of the node
        Node *next; // the pointer to the next node

        // Constructors
        Node();
        Node(Room* data);
        Node(Room* data, Node *next);
};

// Singly Linked List class
class LinkedList {
    private:
        Node *head; // the head of the linked list
        Node *tail; 
        int size; // the size of the linked list
        Node *current; // the current node for traversal

    public:
        // Constructors
        LinkedList();
        LinkedList(Node *head, int size);

        // Destructor
        ~LinkedList();

        // Setters and Getters
        void setHead(Node *head);
        Node* getHead() const;
        Node* getCurrent() const;
        void setSize(int size);
        int getSize() const;
        bool isLast() const;

        void add(Room* data); // adds a node to the linked list
        void next(); // moves to the next node
        void prev(); // moves to the previous node
};

#endif /* LinkedList_hpp */