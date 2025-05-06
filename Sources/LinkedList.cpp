#include "../Headers/LinkedList.hpp"
#include <iostream>

// Node class constructors
Node::Node() : data(nullptr), next(nullptr) {} // Default constructor

Node::Node(Room* data) : data(data), next(nullptr) {} // Constructor with Room data

Node::Node(Room* data, Node *next) : data(data), next(next) {} // Constructor with Room data and next pointer

// LinkedList class constructors and destructor
LinkedList::LinkedList() {
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    size = 0;
}

LinkedList::LinkedList(Node *head, int size) {
    this->head = head;
    this->tail = nullptr; 
    this->current = head;
    this->size = size;
}

LinkedList::~LinkedList() {
    Node *currentptr = head;
    Node *next;

    while (currentptr != nullptr) {
        next = currentptr->next;
        delete currentptr->data;
        delete currentptr;
        currentptr = next;
    }
    head = nullptr;
    tail = nullptr;
    current = nullptr; 
}

// Setters and getters
void LinkedList::setHead(Node *head) {
    this->head = head;
}

Node* LinkedList::getHead() const {
    return head;
}

Node* LinkedList::getCurrent() const {
    return current;
}

void LinkedList::setSize(int size) {
    this->size = size;
}

int LinkedList::getSize() const {
    return size;
}

bool LinkedList::isLast() const {
    return current == nullptr || current->next == nullptr;
}

// Add function (fixed to add to the end of the list)
void LinkedList::add(Room* data) {
    Node *newNode = new Node(data);
    if (!head) { // If the list is empty
        head = newNode;
        tail = newNode;
        current = head; // Initialize current to head
    } else { // Add to the end of the list
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

// Move to the next node
void LinkedList::next() {
    if (current != nullptr) {
        current = current->next;
    }
}

// Move to the previous node
void LinkedList::prev() {
    if (current != head && head != nullptr) { // Ensure current is not at the head
        Node *currentptr = head;
        while (currentptr->next != current) { // Traverse to the node before current
            currentptr = currentptr->next;
        }
        current = currentptr;
    }
}