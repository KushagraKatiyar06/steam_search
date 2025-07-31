//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#ifndef RAPIDFUZZIE_H
#define RAPIDFUZZIE_H
#include <unordered_map>
#include "Game.h"

class RapidFuzzie {
private:
    // Allows matcher to access full list of game names
    const std::unordered_map<std::string, Game>& allGameMetaData;

    // Threshold for similarity
    double similarityThreshold;

public:
    RapidFuzzie(const std::unordered_map<std::string, Game>& allGameMetaData, double similarityThreshold = 75.0); // default constructor, setting base similarity score to 75%
    std::string getMatchedName(); // when user inputs name that's not fully matched, will suggest alternative correct name
};



#endif //RAPIDFUZZIE_H
