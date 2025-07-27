//
// Created by Agnivesh Kaundinya on 7/27/2025.
//

#include "UnorderedSetSC.h"
#include <string>

// Helper
int getNextPrimeHelper(int n) {
    for (int i = 0; i < NUM_PRIMES; i++) {
        if (PRIMES[i] >= n) {
            return PRIMES[i];
        }
    }
    // Fallback if n is larger than the largest predefined prime.
    return PRIMES[NUM_PRIMES - 1];
}

// Constructor
template<typename T>
UnorderedSetSC<T>::UnorderedSetSC(int initialCapacity, double loadFactor) : currSize(0), maxLoadFactor(loadFactor) {
    // Ensure initial capacity is at least a prime from the list
    this->capacity = getNextPrimeHelper(initialCapacity);

    // Allocate Table
    this->table = new Node*[this->capacity];

    // Initialize all buckets to nullptr
    for (int i = 0; i < this->capacity; i++) {
        this->table[i] = nullptr;
    }
}

// Destructor
template<typename T>
UnorderedSetSC<T>::~UnorderedSetSC() {
    clear();
    delete[] table;
    table = nullptr;
}

// Hash Function
template<typename T>
unsigned int UnorderedSetSC<T>::hashFunction(const T &key) const {
    return static_cast<unsigned int>(key) % this->capacity; // modulo should be good enough
}

// Helper : Delete Linked List
template<typename T>
void UnorderedSetSC<T>::deleteList(Node *head) {
    Node* curr = head;
    while (curr != nullptr) {
        Node* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
}

// Helper : Resize Hash Table
template<typename T>
void UnorderedSetSC<T>::resize() {
    int oldCapacity = this->capacity;
    Node** oldTable = this->table;

    // Get a new, larger prime capacity
    this->capacity = getNextPrimeHelper(oldCapacity * 2); // Double capacity and find next prime
    if (this->capacity <= oldCapacity) {  // Handle case where we ran out of primes or it didn't grow
        return;
    }

    // Allocate new table
    this->table = new Node*[this->capacity];
    for (int i = 0; i < this->capacity; i++) {
        this->table[i] = nullptr; // initialize buckets to nullptr
    }

    int oldSize = this->currSize;
    this->currSize = 0;

    // Re-insert original elements
    for (int i = 0 ; i < oldCapacity ; i++) {
        Node* curr = oldTable[i];
        while (curr != nullptr) {
            insert(curr->data);
            curr = curr->next;
        }
    }

    // Deallocate nodes
    for (int i = 0; i < oldCapacity; i++) {
        deleteList(oldTable[i]);
    }
    delete[] oldTable;
}

// Insert
template<typename T>
bool UnorderedSetSC<T>::insert(const T &element) {
    // Check load factor and resize if necessary
    if (static_cast<double>(currSize + 1) / capacity > maxLoadFactor) {
        resize();
    }

    unsigned int index = hashFunction(element);

    // Traverse linked list @ index to check for duplicates
    Node* curr = this->table[index];
    while (curr != nullptr) {
        if (curr->data == element) {
            return false; // element already exists
        }
        curr = curr->next;
    }

    // Element is not found
    Node* newNode = new Node(element);
    newNode->next = this->table[index];
    this->table[index] = newNode;
    this->currSize++;
    return true;
}

// Erase
template<typename T>
bool UnorderedSetSC<T>::erase(const T &element) {
    unsigned int index = hashFunction(element);

    Node* curr = this->table[index];
    Node* prev = nullptr;

    while (curr != nullptr) {
        if (curr->data == element) { // found element remove from list
            if (prev == nullptr) {
                this->table[index] = curr->next; // its at the head
            }
            else { // its in the middle or end
                prev->next = curr->next;
            }
            delete curr; // deallocate the node
            this->currSize--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false; // element not found
}

// Contains
template<typename T>
bool UnorderedSetSC<T>::contains(const T &element) const {
    unsigned int index = hashFunction(element);

    Node* curr = this->table[index];
    while (curr != nullptr) {
        if (curr->data == element) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

// Utility
template<typename T>
int UnorderedSetSC<T>::size() const {
    return this->currSize;
}

template<typename T>
void UnorderedSetSC<T>::clear() {
    for (int i = 0 ; i < this->capacity ; i++) {
        deleteList(this->table[i]); // Delete the linked list in each bucket
        this->table[i] = nullptr; // Set the bucket head back to nullptr
    }
    this->currSize = 0;
}

template<typename T>
double UnorderedSetSC<T>::getLoadFactor() const {
    if (this->capacity == 0) return 0.0;
    return static_cast<double>(this->currSize) / this->capacity;
}

template class UnorderedSetSC<int>;
template class UnorderedSetSC<double>;
template class UnorderedSetSC<std::string>;













