//
// Created by kkati on 7/29/2025.
//

#include "minHash.h"
#include <fstream>
#include <iostream>
#include <random>
#include <ctime>

using namespace std;

//Constructor - creates random hash functions
minHash:: minHash(int n, const unordered_map<string, int>& tags) {
    this->indexedTags = tags;

    //This vectoor has a very simple value of 1-numTags, this is before filling it up
    //with random values based on random hash functions
    vector<int> indicesBeforePermutations;
    for (int i = 0; i < indexedTags.size(); i++) {
        indicesBeforePermutations.push_back(i);
    }

    //Creates N different permutations of the indices. basically shuffling the vector by reordering indices
    srand(time(0));
    for (int i = 0; i < n; i++){
        vector<int> v = indicesBeforePermutations;

        for (int j = v.size() - 1 ; j >= 0; j--) {
            int k = rand() % (j+1);

            int temp = v[j];
            v[j] = v[k];
            v[k] = temp;
        }

        hashCombinations.push_back(v);
    }

}

//Getters
unordered_map<string,int> minHash::getIndexedTags() {
    return indexedTags;
}

vector<vector<int>> minHash::getHashCombinations() {
    return hashCombinations;
}

//Methods

vector<int> minHash::createSignature(const Game& game) {
    int numCombinations = hashCombinations.size();
    int numTags = indexedTags.size();

    //It is important to initialize the vector this way because the value must be larder than any of the possible hash values.
    //Later on there will be comparisons done where the smallest index is prioritized, hence the starting vector has the
    //largest possible value for this particaluar game hashes
    vector<int> signature(numCombinations, numTags + 1);


    //Grabs the indices from the basic tag index map for this particular game
    const auto& gameTagsMap = game.getTags();
    vector<int> gameTagIndices;


    for (const auto& tagData : gameTagsMap) {
        string tagName = tagData.first;
        if (indexedTags.contains(tagName)) {
            gameTagIndices.push_back(indexedTags[tagName]);
        }
    }

    if (gameTagIndices.empty()) {
        return signature;
    }


    //For the number of hash functions that exist, many different hash functions assigned different values
    //for each tag, for this alogirhtmn, we find the smallest possible one, which is the minimum hash.
    for  (int i = 0; i < numCombinations; i++) {

        for (int tagIndex : gameTagIndices) {
            if (hashCombinations[i][tagIndex] < signature[i]) {
                signature[i] = hashCombinations[i][tagIndex];
            }
        }
    }

    return signature;
}

//compares the two signatures that should now contain the minimum hash value of each tag previously made
//works like the regular jaccard algorithm by comparing common values in each spot of the vector by the
//total hash values between both games. The result is essentially an estimate compared to actually using
//tags in the jaccards algorithm, due to randomizing hash functions and values.
double minHash:: miniJaccards(const vector<int>& gameA, const vector<int>& gameB) {

    if (gameA.size() != gameB.size() || gameA.empty()) {
        return 0;
    }

    int identicalValues = 0;
    int numCombinations = gameA.size();

    for (int i = 0; i < numCombinations; i++) {
        if (gameA[i] == gameB[i]) {
            identicalValues++;
        }
    }

    return static_cast<double>(identicalValues) / static_cast<double>(numCombinations);

}
