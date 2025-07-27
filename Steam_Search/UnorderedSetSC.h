//
// Created by Agnivesh Kaundinya on 7/27/2025.
//

#ifndef UNORDEREDSETSC_H
#define UNORDEREDSETSC_H


#include <string>
#include <functional>


// Prime Number Helper for Separate Chaining
static const int PRIMES[] = {7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521};
static const int NUM_PRIMES = sizeof(PRIMES) / sizeof(PRIMES[0]);


int getNextPrimeHelper(int n);

// Hash Function Struct
template <typename T>
struct MyHasherSC { // Renamed to MyHasherSC to distinguish from OA version if both are used
    unsigned int operator()(const T& key, int capacity) const;
};

// Explicit specialization declarations for int
template <>
struct MyHasherSC<int> {
    unsigned int operator()(const int& key, int capacity) const;
};

// Explicit specialization declarations for std::string
template <>
struct MyHasherSC<std::string> {
    unsigned int operator()(const std::string& key, int capacity) const;
};



template<typename T>
class UnorderedSetSC {
private:
    // This struct defines how each element in our linked list looks.
    struct Node {
        T data; // element
        Node* next; // pointer to next

        // Constructor
        Node(const T& val) : data(val), next(nullptr) {}
    };

    // Hash Table Members
    Node** table; // array of pointers to the Nodes

    int capacity;
    int currSize;
    double maxLoadFactor;

    // Private Helpers
    unsigned int hashFunction(const T& key) const;
    void resize(); // Helper to resize the hash table when the load factor exceeds maxLoadFactor.
    void deleteList(Node* head); // Helper to delete all nodes in a linked list starting from a given head.

public:
    // Constructor and Destructor
    UnorderedSetSC(int initialCapacity = 7, double loadFactor = 1.0);
    ~UnorderedSetSC();

    // Core Operations
    bool insert(const T& element);
    bool erase(const T& element);
    bool contains(const T& element) const;
    int size() const;
    bool isEmpty() const;
    void clear();

    // Utlility Functions
    double getLoadFactor() const;
    void printBuckets() const;
};


#endif //UNORDEREDSETSC_H
