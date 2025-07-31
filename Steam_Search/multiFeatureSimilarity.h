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

double calculateWeightedJaccard(
    const std::unordered_map<std::string, int>& tagsA,
    const std::unordered_map<std::string, int>& tagsB);


double calculateJaccard(
    const std::set<std::string>& setA,
    const std::set<std::string>& setB
);


double calculateOverallWeightedSimilarity(
    const Game& gameA,
    const Game& gameB,
    double weightTags,
    double weightPublishers,
    double weightDevelopers,
    double weightGenres);




#endif //MULTIFEATURESIMILARITY_H

// hey