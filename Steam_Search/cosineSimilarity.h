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
    unordered_map<string, int> indexedTags; //assigns all tag a number 1-39 and allows for instant acesss
    unordered_map<string, vector<double>> gameSignatures; //keeps vectors for every game based on voting on tags

public:
    //Constructor
    cosineSimilarity(const unordered_map<string, int>& indexedTags); //initializes local indexedtags

    //Getters
    unordered_map<string, int> getIndexedTags();
    unordered_map<string, vector<double>> getGameSignatures();

    //Methods
    void createGameSignatures(const unordered_map<string, Game>& allData); //creates a vector of all games for their personality profiles dependent on how many votes per tag it recieved
    double similarity(const string& gameA, const string& gameB); //gets the dot product of gameA and gameB's vectors, that product represents the angle between the two (similarity).
};

#endif //COSINESIMILARITY_H
