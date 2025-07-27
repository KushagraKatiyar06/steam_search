//
// Created by Agnivesh Kaundinya on 7/27/2025.
//

#include <string>
#ifndef UNORDEREDSETOA_H
#define UNORDEREDSETOA_H

// Prime Number Helper for Open Adressing
static const int PRIMES[] = {7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521};
static const int NUM_PRIMES = sizeof(PRIMES) / sizeof(PRIMES[0]);

static const int SECOND_HASH_PRIME = 5; // choose another random prime

// Function to get next prime larger than a given value
int getNextPrimeHelper(int n);

// Custom Hash Function Struct
template <typename T>
struct MyHasher {
    unsigned int operator()(const T& key, int capacity) const;
    unsigned int operator()(const T& key, int capacity, int secondHashPrime) const; // For hash2
};

// Explicit specialization declarations for int
template <>
struct MyHasher<int> {
    unsigned int operator()(const int& key, int capacity) const;
    unsigned int operator()(const int& key, int capacity, int secondHashPrime) const;
};

// Explicit specialization declarations for std::string
template <>
struct MyHasher<std::string> {
    unsigned int operator()(const std::string& key, int capacity) const;
    unsigned int operator()(const std::string& key, int capacity, int secondHashPrime) const;
};


template<typename T>
class UnorderedSetOA {
private:
    // Internal Entry Structure
    enum EntryStatus {
        EMPTY,
        OCCUPIED,
        DELETED,
    };

    struct Entry {
        T data;
        EntryStatus status;

        // Constructor
        Entry() : status(EMPTY) {}
        Entry(const T& val) : data(val), status(OCCUPIED) {}
    };

    // Hash Table Members
    Entry* table;
    int capacity;
    int currSize;
    int deletedCount;
    double maxLoadFactor; // Threshold for resizing based on (currentSize + deletedCount) / capacity
    double deleteThresholdFactor; // If deletedCount is too high, resize/rehash to clean up.

    // Private Helper Functions
    unsigned int hashFunction1(const T& key) const;
    unsigned int hashFunction2(const T& key) const;

    // Returns the index where the element IS or WHERE IT SHOULD BE INSERTED.
    // out_found: A pointer to a boolean that will be set to true if the element is found, false otherwise.
    int findProbeIndex(const T& key, bool* out_found = nullptr) const;

    void resize(bool forceRehash = false);
public:
    // Constructor and Destructor
    UnorderedSetOA(int initialCapacity = 7, double loadFactor = 0.7, double deleteFactor = 0.5);
    ~UnorderedSetOA();

    // Core Operations
    bool insert(const T& element);
    bool erase(const T& element);
    bool contains(const T& element) const;
    int size() const;
    bool isEmpty() const;
    void clear();

    // Utility
    double getLoadFactor() const;
    double getDeletedFactor() const;
    // void printTable() const;
};



#endif //UNORDEREDSETOA_H
