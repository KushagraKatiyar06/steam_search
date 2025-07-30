//
// Created by bmmah on 7/30/2025.
//

#ifndef ALGORITHMS_B_H
#define ALGORITHMS_B_H

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include "Game.h"
#include "jaccardsSimilarity.h"

using namespace std;

enum BucketLevel {
    HIGH_RELEVANCE,
    MEDIUM_RELEVANCE,
    TAG_SIMILAR,
    WEAK_SIMILAR,
    LOW_RELEVANCE
};

BucketLevel setBucket(string& selected, string& candidate, unordered_map<string, Game>& gameData, unordered_map<string, double> scores);
vector<string> decisionTree(string& selected, unordered_map<string, Game>& gameData, unordered_map<string, string> decoder);


#endif //ALGORITHMS_B_H
