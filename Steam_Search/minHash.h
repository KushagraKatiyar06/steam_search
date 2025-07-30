//
// Created by kkati on 7/29/2025.
//

#ifndef MINHASH_H
#define MINHASH_H

#include <vector>
#include <string>
#include <unordered_map>

#include "Game.h"

using namespace std;

class minHash {
private:
    unordered_map<string, int> indexedTags;
    vector<vector<int>> hashCombinations;

public:
    //Constructor
    minHash(int n, const unordered_map<string, int>& indexedTags); // will ta ke the tags.txt file and populate unique tags vector. also will initialize n hash functions

    //Getters
    unordered_map<string,int> getIndexedTags();
    vector<vector<int>> getHashCombinations();

    //Methods
    vector<int> createSignature(const Game& game); //creates ONE signature for a game based on tags
    double miniJaccards(const vector<int>& gameA, const vector<int>& gameB); //compare signatures of two games to determine similarity

};


#endif //MINHASH_H
