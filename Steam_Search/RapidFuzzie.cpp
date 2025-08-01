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
    cout << "\nPlease input the game you'd like us to search: " << endl;
    string inputGN;
    getline(cin, inputGN); // read line of full game name
    inputGN.erase(inputGN.find_last_not_of(" \t\r\n") + 1); // remove trailing or leading whitespace

    if (inputGN == "q" || inputGN == "Q") {
        return "q";
    }


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
    // Store all matches that meet the similarity threshold
    std::vector<std::pair<double, std::string>> potentialMatches;

    // Iterate through all available names to find matches
    for (const std::string& currChoice : allGameNames) {
        // Normalize database game name to lowercase
        std::string normalizedChoice = currChoice;
        std::transform(normalizedChoice.begin(), normalizedChoice.end(), normalizedChoice.begin(), [](unsigned char c){return tolower(c);});

        // Calculate Similarity Scores
        double currScore = rapidfuzz::fuzz::ratio(normalizedInputGN, normalizedChoice);

        // If the score meets the threshold, add to potential matches
        if (currScore >= similarityThreshold) {
            potentialMatches.push_back({currScore, currChoice});
        }
    }

    // Sort matches by score in descending order
    std::sort(potentialMatches.begin(), potentialMatches.end(), [](const auto& a, const auto& b) {return a.first > b.first;});

    // Define how many suggestions to print
    int maxSuggestionSize = 3;

    if (!potentialMatches.empty()) {
        std::cout << "\nDid you mean one of these?" << endl;
        std::cout << "----------------------------" << endl;
        std::string selectedGameName = ""; // to store final selection

        for (int i = 0; i < potentialMatches.size() && i < maxSuggestionSize; i++) {
            std::cout << (i + 1) << ". '" << potentialMatches[i].second << "' (Similarity: " << std::fixed << std::setprecision(2) << potentialMatches[i].first << "%)" << endl;
        }

        // Prompt user to select choice
        std::cout << "----------------------------" << endl;
        std::cout << "Enter the number of choice, or 0 to re-enter: ";
        int choiceNum;
        std::cin >> choiceNum;


        // Process choice
        if (choiceNum > 0 && choiceNum <= potentialMatches.size() && choiceNum <= maxSuggestionSize) {
            selectedGameName = potentialMatches[choiceNum - 1].second;
            success = true;
            std::cout << "You selected: '" << selectedGameName << "'." << endl;
            return selectedGameName;
        }
        else if (choiceNum == 0) {
            std::cout << "Re-entering game name..." << std::endl;
            success = false;
            return getMatchedName();
        }
        else {
            std::cout << "Invalid choice Please re-enter a valid selection." << std::endl;
            success = false;
            return getMatchedName();
        }
    }
    else {
        // No matches found above similarity threshold
        std::cout << "No close matches found for the input above " << std::fixed << std::setprecision(2) << similarityThreshold << "% similarity." << std::endl;
        std::cout << "Please try again with a more accurate name." << std::endl;

        // Offer to re-enter or return empty
        std::cout << "Enter 0 to re-enter game now, or press any other key to exit: ";
        int choiceNum;
        std::cin >> choiceNum;


        if (choiceNum == 0) {
            success = false;
            return getMatchedName();
        }
        else {
            success = false;
            return "";
        }
    }
}
