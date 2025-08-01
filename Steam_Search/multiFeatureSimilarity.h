//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#ifndef MULTIFEATURESIMILARITY_H
#define MULTIFEATURESIMILARITY_H

#include <string>
#include <unordered_map>
#include <set>
#include "Game.h"

using namespace std;

// Calculates Weighted Jaccard Similarity for tags.
double calculateWeighted(const std::unordered_map<std::string, int>& tagsA, const std::unordered_map<std::string, int>& tagsB);

// Calculates standard Jaccard Similarity between two sets of strings.
// Used for features like publishers, developers, categories, genres.
double calculateJaccard(const set<string>& setA, const set<string>& setB);

// Calculates similarity based on the absolute difference between two review scores.
double calculateReviewScoreSimilarity(double scoreA, double scoreB);

// Calculates the overall weighted similarity between two games based on multiple features.
double calculateOverallWeightedSimilarity(const Game& gameA, const Game& gameB, double weightTags, double weightPublishers, double weightDevelopers, double weightReviewScore);


#endif //MULTIFEATURESIMILARITY_H
