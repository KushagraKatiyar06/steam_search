//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#include "RapidFuzzie.h"
#include <iostream>
#include<vector>
#include <algorithm>
#include <iomanip> // for precision setting
#include <limits> // for std::numeric_limits
#include <cctype> // for tolower

#include <rapidfuzz/fuzz.hpp>

// Constructor Initalizes values
RapidFuzzie::RapidFuzzie(const std::unordered_map<std::string, Game> &metaData, double threshold) : allGameMetaData(metaData), similarityThreshold(threshold){}

std::string RapidFuzzie::getMatchedName() {
    cout << "Enter game name: ";
    std::string inputGN;
    std::getline(std::cin, inputGN); // read line of full game name

    // Normalize user input to lowercase
    std::string normalizedInputGN = inputGN;
    std::transform(normalizedInputGN.begin(), normalizedInputGN.end(), normalizedInputGN.begin(),[](unsigned char c){return std::tolower(c);}); // converts individual characters to lower

    std::vector<std::string> allGameNames; // prepare a list of all the game names in the JSON
    for (const auto& pair : allGameMetaData) {
        allGameNames.push_back(pair.first);
    }

    if (allGameNames.empty()) { // if it's empty then return
        return "";
    }

    // Matching Logic
    double bestScore = -1.0; // this is the base value
    std::string bestMatch;
    bool matchFound = false;

    // Iterate through all available names to find best match
    for (const std::string& currChoice : allGameNames) {
        // normalize database game name
        std::string normalizedCurrChoice = currChoice;
        std::transform(normalizedCurrChoice.begin(), normalizedCurrChoice.end(), normalizedCurrChoice.begin(),[](unsigned char c){return std::tolower(c);}); // same transformation as for input

        // calculate similarity score
        double currScore = rapidfuzz::fuzz::ratio(normalizedInputGN, normalizedCurrChoice);

        if (currScore > bestScore) {
            bestScore = currScore;
            bestMatch = currChoice;
            matchFound = true;
        }
    }

    if (matchFound) {
        if (bestScore >= similarityThreshold) {
            cout << "Did you mean '" << bestMatch << "'?" << endl;
            return bestMatch;
        }
        else {
            cout << "No matches close to or above similarity threshold" << endl;
        }
    }
    else {
        cout << "No matches in game directory" << endl;
        return "";
    }

}

