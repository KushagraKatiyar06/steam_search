#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <nlohmann/json.hpp>
#include "Game.h"

#include "readJson.h"
#include "jaccardsSimilarity.h"
#include "multiFeatureSimilarity.h"
#include "algorithms_B.h"


using json = nlohmann::json;
using namespace std;

// TODO: implement fuzzy matching or other normalizer so users can find games like The Crew™ 2. They would type The Crew 2 and be prompted "did you mean The Crew™ 2." These names have special characters normal users won't be able to input.
// also look into creating a drop down, as the user types in the name of their game the drop down gets shorter and shorter helping them hone in
// TODO: make names case insensitive and whitsepace insensitive

// TODO: deal with foreign languages not printing properly

// TODO: added supported language logic to all of our algorithms (if other game langs doesn't contain supported game langs) continue;

int main()
{
    cout << "Prepping dataset, and preprocessing data for algorithms" << endl;
    ifstream f("../steam_games.json"); // use ../steam_games_less.json for testing runs
    // Check if the file opened successfully
    if (!f.is_open()) {
        cout << "Error: Could not open given JSON. Please ensure the file exists and the path is correct." << endl;
        return 1; // Exit the program if the file cannot be opened
    }
    json dataJSON = json::parse(f);
    cout << "Loaded " << dataJSON.size() << " games from JSON." << endl;
    string source;

    unordered_map<string, Game> metaData;
    cout << "Calling readJson to populate game data..." << endl;
    readJson(dataJSON, metaData); // Populate your game data map
    cout << "Finished populating game data. Total games in metaData: " << metaData.size() << endl;


    // --- Multi-Feature Weighted Similarity: Top 10 Games ---
    cout << "\n--- Multi-Feature Weighted Similarity: Top 10 Games ---" << endl;

    string sourceGameName = "Marvel's Spider-Man 2"; // Choose  source game here

    // Check if the source game exists
    if (metaData.count(sourceGameName) == 0) {
        cout << "Error: Source game '" << sourceGameName << "' not found in dataset. Cannot perform similarity search." << endl;
    } else {
        const Game& sourceGame = metaData[sourceGameName];

        // Define importance weights for the multi-feature algorithm
        double weightTags = 0.5;
        double weightPublishers = 0.1;
        double weightDevelopers = 0.1;
        double weightReviewScore = 0.3;

        cout << "\nFinding top 10 similar games to: '" << sourceGameName << "'" << endl;
        cout << "Using Weights: Tags=" << weightTags << ", Publishers=" << weightPublishers
             << ", Developers=" << weightDevelopers << ", Review Score=" << weightReviewScore << endl;
        cout << "----------------------------------------------------" << endl;

        // Max-heap to store (similarity, gameName) pairs, ordered by similarity (highest first)
        priority_queue<pair<double, string>> topSimilarGames;

        // Iterate through all other games in metaData
        for (const auto& pair : metaData) {
            const string& compareGameName = pair.first;
            // Skip comparing the game with itself
            if (compareGameName == sourceGameName) {
                continue;
            }

            const Game& compareGame = pair.second;

            // Calculate the overall weighted similarity
            double similarity = calculateOverallWeightedSimilarity(
                sourceGame, compareGame,
                weightTags, weightPublishers, weightDevelopers, weightReviewScore
            );

            // Add to the priority queue
            topSimilarGames.emplace(similarity, compareGameName);
        }

        // Print the top 10 similar games
        cout << "\nTop 10 most similar games to '" << sourceGameName << "':" << endl;
        cout << "--------------------------" << endl;
        for (int i = 0; i < 10 && !topSimilarGames.empty(); ++i) {
            pair<double, string> top = topSimilarGames.top();
            cout << "Similarity: " << fixed << setprecision(4) << top.first << " | Game:  " << top.second << endl;
            topSimilarGames.pop();
        }
    }
    cout << "--- End of Multi-Feature Weighted Similarity Test ---" << endl;
    /*
    // update decoder from file, this allows us to map gameIDs to gameNames for quick lookup
    unordered_map<string, string> decoder;
    ifstream inFile("../decoder.txt");
    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        if (string key, value; getline(iss, key, '\t') && getline(iss, value)) {
            decoder[key] = value;
        }
    }

    cout << "Please input the game you'd like us to search: " << endl;
    getline(cin >> std::ws, source);
    source.erase(source.find_last_not_of(" \t\r\n") + 1);
    cout << "How many games would you like displayed at a time: " << endl;
    int num_games;
    cin >> num_games;
    string response;

    // decision tree
    vector<string> rankings = decisionTree(source, metaData, decoder);
    int i;
    for (i = 0; i < num_games; i++)
    {
        cout << rankings[i] << endl;
    }
    cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
    cin >> response;
    while (response == "m")
    {
        if (i >= rankings.size())
        {
            cout << "No more games to display." << endl;
        }
        for (int j = 0; j < num_games && i < rankings.size(); j++, i++)
        {
            cout << rankings[i] << endl;
        }
        cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
        cin >> response;
    }


    // jaccards
    /*string compare;
    priority_queue<pair<double, string>> maxHeap;
    for (const auto& [key, value] : decoder) {
        if (value != source) {
            compare = value;
        }
        else
        {
            continue;
        }
        maxHeap.emplace(jaccardsSimilarityWeighted(source, compare, metaData), value);
    }
    for (int i = 0; i < num_games ; i++) {
        cout << maxHeap.top().first << " : " << maxHeap.top().second << endl;
        maxHeap.pop();
    }
    cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
    cin >> response;
    while (response == "m")
    {
        if (maxHeap.empty())
        {
            cout << "No more games to display." << endl;
        }
        for (int i = 0; i < num_games ; i++) {
            cout << maxHeap.top().first << " : " << maxHeap.top().second << endl;
            maxHeap.pop();
        }
        cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
        cin >> response;
    }

    */

    return 0;
}