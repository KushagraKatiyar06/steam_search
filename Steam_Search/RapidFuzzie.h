//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#ifndef RAPIDFUZZIE_H
#define RAPIDFUZZIE_H
#include <unordered_map>
#include "Game.h"

using namespace std;

class RapidFuzzie {
private:
    // Allows matcher to access full list of game names
    const unordered_map<string, Game>& allGameMetaData;

    // Threshold for similarity
    double similarityThreshold;

    bool success;
public:
    RapidFuzzie(const unordered_map<string, Game>& allGameMetaData, double similarityThreshold = 75.0); // default constructor, setting base similarity score to 75%
    string getMatchedName(); // when user inputs name that's not fully matched, will suggest alternative correct name
    bool getSuccess();
};



#endif //RAPIDFUZZIE_H
