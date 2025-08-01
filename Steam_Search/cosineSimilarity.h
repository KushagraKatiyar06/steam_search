//
// Created by kkati on 7/30/2025.
//

#ifndef COSINESIMILARITY_H
#define COSINESIMILARITY_H

#include <unordered_map>
#include <string>
#include <vector>

#include "Game.h"

using namespace std;

class cosineSimilarity {
private:
    unordered_map<string, int> indexedTags;
    unordered_map<string, vector<double>> gameSignatures;

public:
    //Constructor
    cosineSimilarity(const unordered_map<string, int>& indexedTags);

    //Getters
    unordered_map<string, int> getIndexedTags();
    unordered_map<string, vector<double>> getGameSignatures();

    //Methods
    void createGameSignatures(const unordered_map<string, Game>& allData);
    double similarity(const string& gameA, const string& gameB);
};

#endif //COSINESIMILARITY_H
