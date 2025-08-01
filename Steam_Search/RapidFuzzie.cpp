//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#include "RapidFuzzie.h"
#include <iostream>
#include<vector>
#include <algorithm>
#include <iomanip> // for precision setting
#include <limits> // for numeric_limits
#include <cctype> // for tolower

#include <rapidfuzz/fuzz.hpp>

// Constructor Initalizes values
RapidFuzzie::RapidFuzzie(const unordered_map<string, Game> &metaData, double threshold) : allGameMetaData(metaData), similarityThreshold(threshold)
{
    success = false;
}

bool RapidFuzzie::getSuccess()
{
    return success;
}

string RapidFuzzie::getMatchedName() {
    cout << "Please input the game you'd like us to search: " << endl;
    string inputGN;
    getline(cin, inputGN); // read line of full game name
    inputGN.erase(inputGN.find_last_not_of(" \t\r\n") + 1); // remove trailing or leading whitespace

    if (allGameMetaData.contains(inputGN)) {
        success = true;
        return inputGN;
    }

    // Normalize user input to lowercase
    string normalizedInputGN = inputGN;
    transform(normalizedInputGN.begin(), normalizedInputGN.end(), normalizedInputGN.begin(),[](unsigned char c){return tolower(c);}); // converts individual characters to lower

    vector<string> allGameNames; // prepare a list of all the game names in the JSON
    for (const auto& pair : allGameMetaData) {
        allGameNames.push_back(pair.first);
    }

    if (allGameNames.empty()) { // if it's empty then return
        return "";
    }

    // Matching Logic
    double bestScore = -1.0; // this is the base value
    string bestMatch;
    bool matchFound = false;

    // Iterate through all available names to find best match
    for (const string& currChoice : allGameNames) {
        // normalize database game name
        string normalizedCurrChoice = currChoice;
        transform(normalizedCurrChoice.begin(), normalizedCurrChoice.end(), normalizedCurrChoice.begin(),[](unsigned char c){return tolower(c);}); // same transformation as for input

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
            cout << "Did you mean '" << bestMatch << "'? [y/n]" << endl;
            string response;
            getline(cin, response);
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush leftover newline

            if (response == "y" || response == "Y")
            {
                success = true;
                return bestMatch;
            }
            return "";
        }
        else {
            cout << "No matches close to or above similarity threshold" << endl;
            return "";
        }
    }
    else {
        cout << "No matches in game directory" << endl;
        return "";
    }
    return "";
}
