//
// Created by Agnivesh Kaundinya on 7/27/2025.
//

#include "UnorderedSetOA.h"

// Global Helper
int getNextPrimeHelper(int n) {
    for (int i = 0; i < NUM_PRIMES; i++) {
        if (PRIMES[i] >= n) {
            return PRIMES[i];
        }
    }
    // Fallback if n is larger than our largest predefined prime.
    return PRIMES[NUM_PRIMES - 1];
}

// MyHasher specialization
unsigned int MyHasher<int>::operator()(const int &key, int capacity) const {
    // basic hash for int
    return static_cast<unsigned int>(key) % capacity;
}

unsigned int MyHasher<int>::operator()(const int &key, int capacity, int secondHashPrime) const {
    // Double Hashing
    return secondHashPrime - (static_cast<unsigned int>(key) % secondHashPrime);
}

// String MyHasher
unsigned int MyHasher<std::string>::operator()(const std::string &key, int capacity) const {
    return std::hash<std::string>{}(key) % capacity;
}

unsigned int MyHasher<std::string>::operator()(const std::string &key, int capacity, int secondHashPrime) const {
    unsigned int hashVal = std::hash<std::string>{}(key);
    return secondHashPrime - (hashVal % secondHashPrime);
}



// UnorderedSetOA Class Methods

// Constructor
template<typename T>
UnorderedSetOA<T>::UnorderedSetOA(int initialCapacity, double loadFactor, double deleteFactor) : currSize(0), deletedCount(0), maxLoadFactor(loadFactor), deleteThresholdFactor(deleteFactor) {
    // Ensure initialCapacity is a prime from the list
    this->capacity = getNextPrimeHelper(initialCapacity);

    // Allocate the table
    this->table = new Entry[this->capacity];
}

// Destructor
template<typename T>
UnorderedSetOA<T>::~UnorderedSetOA() {
    delete[] table;
    table = nullptr;
}

// Hash Function
template<typename T>
unsigned int UnorderedSetOA<T>::hashFunction1(const T &key) const {
    return MyHasher<T>()(key, this->capacity);
}

template<typename T>
unsigned int UnorderedSetOA<T>::hashFunction2(const T &key) const {
    return MyHasher<T>()(key, this->capacity, SECOND_HASH_PRIME);
}

// Probe Index
template<typename T>
int UnorderedSetOA<T>::findProbeIndex(const T &key, bool *out_found) const {
    unsigned int initialIndex = hashFunction1(key);
    unsigned int step = hashFunction2(key);
    int firstDeletedIndex = -1;

    for (int i = 0; i < this->capacity; i++) {
        int currIndex = (initialIndex + i * step) % this->capacity;
        Entry& entry = this->table[currIndex];

        if (entry.status == OCCUPIED) {
            if (entry.data == key) {
                if (out_found) *out_found = true;
                return currIndex;
            }
        }
        else if (entry.status == EMPTY) {
            if (out_found) *out_found = false;
            return (firstDeletedIndex != -1) ? firstDeletedIndex : currIndex;
        }
        else if (entry.status == DELETED) {
            if (firstDeletedIndex == -1) {
                firstDeletedIndex = currIndex;
            }
        }
    }
    if (out_found) *out_found = false;
    // will likely not reach here if the load factor is managed
    return (firstDeletedIndex != -1) ? firstDeletedIndex : -1;
}

// Resize
template<typename T>
void UnorderedSetOA<T>::resize(bool forceRehash) {
    bool needResize= (static_cast<double>(currSize + deletedCount) / capacity > maxLoadFactor);
    bool needsRehashForDeleted = (static_cast<double>(deletedCount) / capacity > deleteThresholdFactor);

    if (!needResize && !needsRehashForDeleted) {
        return;
    }

    int oldCapacity = this->capacity;
    Entry* oldTable = this->table;

    this->capacity = getNextPrimeHelper(oldCapacity * 2);
    if (this->capacity <= oldCapacity) {
        return;
    }

    this->table = new Entry[this->capacity];
    this->currSize = 0;
    this->deletedCount = 0;

    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].status == OCCUPIED) {
            insert(oldTable[i].data);
        }
    }
    delete[] oldTable;
}

// Insert
template<typename T>
bool UnorderedSetOA<T>::insert(const T &element) {
    if (static_cast<double>(currSize + deletedCount + 1) / capacity > maxLoadFactor || static_cast<double>(deletedCount) / capacity > deleteThresholdFactor) {
        resize();
    }

    bool found = false;
    int index = findProbeIndex(element, &found);

    if (found) {
        return false;
    }

    if (this->table[index].status == DELETED) {
        this->deletedCount--;
    }

    this->table[index].data = element;
    this->table[index].status = OCCUPIED;
    this->currSize++;

    return true;
}

// Erase
template<typename T>
bool UnorderedSetOA<T>::erase(const T &element) {
    bool found = false;
    int index = findProbeIndex(element, &found);

    if (!found) {
        return false;
    }

    this->table[index].status = DELETED;
    this->deletedCount++;
    this->currSize--;

    if (static_cast<double>(deletedCount) / capacity > deleteThresholdFactor) {
        resize(true);
    }

    return true;
}

// Contains
template<typename T>
bool UnorderedSetOA<T>::contains(const T &element) const {
    bool found = false;
    findProbeIndex(element, &found);
    return found;
}

// Size
template<typename T>
int UnorderedSetOA<T>::size() const {
    return this->currSize;
}

// isEmpty
template<typename T>
bool UnorderedSetOA<T>::isEmpty() const {
    return this->currSize == 0;
}

// Clear
template<typename T>
void UnorderedSetOA<T>::clear() {
    for (int i = 0; i < this->capacity; i++) {
        this->table[i].status = EMPTY;
    }
    this->currSize = 0;
    this->deletedCount = 0;
}

// getLoadFactor
template<typename T>
double UnorderedSetOA<T>::getLoadFactor() const {
    if (this->capacity == 0) return 0.0;
    return static_cast<double>(this->currSize) / this->capacity;
}

// getDeletedFactor
template<typename T>
double UnorderedSetOA<T>::getDeletedFactor() const {
    if (this->capacity == 0) return 0.0;
    return static_cast<double>(this->deletedCount) / this->capacity;
}

// Explicit Instantiation
template class UnorderedSetOA<int>;
template class UnorderedSetOA<std::string>;












