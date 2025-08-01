//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#ifndef MULTIFEATURESIMILARITY_H
#define MULTIFEATURESIMILARITY_H

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "Game.h"
#include "jaccardsSimilarity.h"

using namespace std;

double jaccardsSimilarityModified(const set<string>& setA, const set<string>& setB);

double calculateOverallWeightedSimilarity(const Game& gameA, const Game& gameB, double weightTags, double weightPublishers, double weightDevelopers, double weightReviewScore, unordered_map<string, Game>& gameData, unordered_map<string, string> decoder);




#endif //MULTIFEATURESIMILARITY_H
