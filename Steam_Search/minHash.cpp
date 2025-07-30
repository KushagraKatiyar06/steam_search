//
// Created by kkati on 7/29/2025.
//

#include "minHash.h"
#include <fstream>
#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>


using namespace std;

//Constructor - sets up map with all tags, and creates signatures for all games
minHash:: minHash(const string& file, int n) {

    //put tags in map
    ifstream tagFile(file);
    if (!tagFile.is_open()) {
        cerr << "error in opening tag file" << endl;
    }

    string currentTag;
    int index = 0;

    while (getline(tagFile, currentTag)) {
        indexedTags[currentTag] = index;
        index++;
    }


    if (indexedTags.empty()) {
        return;
    }

    //create hash combinations
    vector<int> indicesBeforePermutations;
    for (int i = 0; i < indexedTags.size(); i++) {
        indicesBeforePermutations.push_back(i);
    }

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
    vector<int> signature(numCombinations, numTags + 1);

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

    for  (int i = 0; i < numCombinations; i++) {

        for (int tagIndex : gameTagIndices) {
            if (hashCombinations[i][tagIndex] < signature[i]) {
                signature[i] = hashCombinations[i][tagIndex];
            }
        }
    }

    return signature;
}

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
