#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <nlohmann/json.hpp>
#include <rapidfuzz/fuzz.hpp>
#include "Game.h"

#include "Game.h"
#include "readJson.h"
#include "minHash.h"
#include "jaccardsSimilarity.h"
#include "cosineSimilarity.h"
#include "multiFeatureSimilarity.h"
#include "algorithms_B.h"
#include "RapidFuzzie.h"


using json = nlohmann::json;
using namespace std;

// NOTICE: the CLI does not print foreign language or special characters correctly. Keep that in mind when there is
// input that looks unintelligible. That is always an issue with the CLI.

// TODO: implement fuzzy matching or other normalizer so users can find games like The Crew™ 2. They would type The
// TODO Crew 2 and be prompted "did you mean The Crew™ 2." These names have special characters normal users won't be able to input.
// TODO also look into creating a drop down, as the user types in the name of their game the drop down gets shorter and
// TODO shorter helping them hone in

// TODO: make names case insensitive and whitsepace insensitive

int main()
{
    // TODO: if it's possible make this disappear from CLI when the prepping is done
    cout << "Prepping dataset, and preprocessing data for algorithms" << endl;
    ifstream f("../games.json"); // use ../games_less.json for testing runs
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
    cout << "Finished populating game data." << endl;

    string tagFile = "../tags.txt";
    unordered_map<string, int> indexedTags = readTags(tagFile);

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

    string response;
    while (response != "q")
    {
        bool invalid = true;
        while (invalid)
        {
            cout << "Please input the game you'd like us to search: " << endl;
            getline(cin >> std::ws, source);
            source.erase(source.find_last_not_of(" \t\r\n") + 1);
            if (!metaData.contains(source)) {
                // TODO: add fuzzy matching logic here
                cout << "Error: Source game '" << source << "' not found in dataset. Cannot perform similarity search." << endl;
            }
            else
            {
                invalid = false;
            }
          
          // RapidFuzzy implementation
          RapidFuzzie fuzzie(metaData, 75.0);

          // call function to get correct game name
          string sourceGameName = fuzzie.getMatchedName();

          if (sourceGameName.empty()) {
              cout << "Invalid Name." << endl;
              return 0;
          }
        }
        cout << "What algorithm would you like for us to use: \n0 - Jaccard's Tag Similarity\n1 - Weighted Jaccard's Tag Similarity\n2 - Rule Based Decision Tree\n3 - Min Hashing\n4 - Cosine Similarity\n5 - Multi-Feature Similarity" << endl;
        int choice;
        cin >> choice;
        cout << "How many games would you like displayed at a time: " << endl;
        int num_games;
        cin >> num_games;


        // declaring all varaibles used within the switch-case, needed as switches don't allow object declaration
        string compare;
        int i;
        priority_queue<pair<double, string>> maxHeap;
        vector<string> rankings;
        priority_queue<pair<double, string>> savedHeap;
        algorithms_b DecisionTree;
        priority_queue<pair<double,string>> similarGames;
        unordered_map<string, vector<int>> allSignatures;
        vector<int>* sourceSignature = nullptr;
        minHash minHash(150, indexedTags);
        priority_queue<pair<double, string>> cosineHeap;
        cosineSimilarity cosineSim(indexedTags);
        Game* sourceGame = nullptr;
        // Max-heap to store (similarity, gameName) pairs, ordered by similarity (highest first)
        priority_queue<pair<double, string>> topSimilarGames;
        // Define importance weights for the multi-feature algorithm
        double weightTags = 0.5;
        double weightPublishers = 0.1;
        double weightDevelopers = 0.1;
        double weightReviewScore = 0.3;
        // TODO: redo all of the ascii formatting so that CLI interface looks consistent across algos
        switch(choice)
        {
            case 0: // Jaccards (unweighted)
                // clears maxHeap if necessary
                while(!maxHeap.empty())
                {
                    maxHeap.pop();
                }
                for (const auto& [key, value] : decoder) {
                    if (value != source) {

                        compare = value;
                    }
                    else
                    {
                        continue;
                    }
                    maxHeap.emplace(jaccardsSimilarity(source, compare, metaData), value);
                }
                for (i = 0; i < num_games ; i++) {
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
                    for (i = 0; i < num_games ; i++) {
                        cout << maxHeap.top().first << " : " << maxHeap.top().second << endl;
                        maxHeap.pop();
                    }
                    cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                    cin >> response;
                }
                break;

            case 1: // Jaccards (weighted)
                // clears maxHeap if necessary
                while(!maxHeap.empty())
                {
                    maxHeap.pop();
                }
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
                for (i = 0; i < num_games ; i++) {
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
                    for (i = 0; i < num_games ; i++) {
                        cout << maxHeap.top().first << " : " << maxHeap.top().second << endl;
                        maxHeap.pop();
                    }
                    cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                    cin >> response;
                }
                break;

            case 2: // Decision Tree
                rankings = DecisionTree.decisionTree(source, metaData, num_games);
                for (i = 0; i < num_games; i++)
                {
                    cout << rankings[i] << endl;
                }
                cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                cin >> response;
                while (response == "m")
                {
                    if (i >= rankings.size() && !DecisionTree.isHeapEmpty())
                    {
                        cout << "No more games to display in this ranking. Would you like to calculate more? [y/n]" << endl;
                        cin >> response;
                        if (response == "y")
                        {
                            i = 0;
                            rankings = DecisionTree.decisionTreeNext(source, metaData, num_games);
                            for (int j = 0; j < num_games && i < rankings.size(); j++, i++)
                            {
                                cout << rankings[i] << endl;
                            }
                            response = "m";
                            continue;
                        }
                        cout << "q - quit; r - return to the main menu" << endl;
                        cin >> response;
                        continue;
                    }
                    if (DecisionTree.isHeapEmpty())
                    {
                        cout << "No more games to display" << endl;
                    }
                    for (int j = 0; j < num_games && i < rankings.size(); j++, i++)
                    {
                        cout << rankings[i] << endl;
                    }
                    cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                    cin >> response;
                }
                break;

            case 3: // Min Hash

                    //minhashing preprep
                    allSignatures.clear();

                    for (const auto& pair : metaData) {
                        const string& gameName = pair.first;
                        const Game& game = pair.second;
                        allSignatures[gameName] = minHash.createSignature(game);
                    }
                    sourceSignature = &allSignatures[source];
                    // clears similarGames if necessary
                    while(!similarGames.empty())
                    {
                        similarGames.pop();
                    }
                    for (const auto& pair : allSignatures) {
                        const string& compareGameName = pair.first;
                        if (compareGameName == source) {
                            continue;
                        }
                        const vector<int>& compareSignature = pair.second;
                        double similarity = minHash.miniJaccards(*sourceSignature, compareSignature);
                        similarGames.emplace(similarity, compareGameName);
                    }

                    for (i = 0; i < num_games; ++i) {
                        // TODO: zero extend the top.first so that formatting looks consistent
                        cout << "Similarity: " << similarGames.top().first << "  |  Game: " << similarGames.top().second << endl;
                        similarGames.pop();
                    }
                    cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                    cin >> response;
                    while (response == "m")
                    {
                        if (similarGames.empty())
                        {
                            cout << "No more games to display." << endl;
                        }
                        for (i = 0; i < num_games ; i++) {
                            cout << "Similarity: " << similarGames.top().first << "  |  Game: " << similarGames.top().second << endl;
                            similarGames.pop();
                        }
                        cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                        cin >> response;
                    }
                break;

            case 4: // Cosine Similarity
                cosineSim.createGameSignatures(metaData);

                for (const auto& pair : metaData) {
                    if (pair.first == source) {
                        continue;
                    }

                    double similarity = cosineSim.similarity(source, pair.first);
                    cosineHeap.emplace(similarity, pair.first);
                }

                for (i = 0; i < num_games; ++i) {
                    cout << "Similarity: " << cosineHeap.top().first << "  |  Game: " << cosineHeap.top().second  << endl;
                    cosineHeap.pop();
                }
                cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                cin >> response;
                while (response == "m")
                {
                    if (cosineHeap.empty())
                    {
                        cout << "No more games to display." << endl;
                    }
                    for (i = 0; i < num_games ; i++) {
                        cout << "Similarity: " << cosineHeap.top().first << "  |  Game: " << cosineHeap.top().second  << endl;
                        cosineHeap.pop();
                    }
                    cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                    cin >> response;
                }
                break;

            case 5: // Multi-Feature Similarity
                sourceGame = &metaData[source];


                cout << "\nFinding similar games to: '" << source << "'" << endl;
                cout << "Using Weights: Tags=" << weightTags << ", Publishers=" << weightPublishers << ", Developers=" << weightDevelopers << ", Review Score=" << weightReviewScore << endl;
                cout << "----------------------------------------------------" << endl;



                // Iterate through all other games in metaData
                for (const auto& pair : metaData) {
                    const string& compareGameName = pair.first;
                    // Skip comparing the game with itself
                    if (compareGameName == source) {
                        continue;
                    }

                    const Game& compareGame = pair.second;

                    // Calculate the overall weighted similarity
                    double similarity = calculateOverallWeightedSimilarity(*sourceGame, compareGame, weightTags, weightPublishers, weightDevelopers, weightReviewScore, metaData, decoder);

                    // Add to the priority queue
                    topSimilarGames.emplace(similarity, compareGameName);
                }
                // prints similar games
                for (i = 0; i < num_games; ++i) {
                    cout << "Similarity: " << fixed << setprecision(4) << topSimilarGames.top().first << " | Game:  " << topSimilarGames.top().second << endl;
                    topSimilarGames.pop();
                }
                cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                cin >> response;
                while (response == "m")
                {
                    if (topSimilarGames.empty())
                    {
                        cout << "No more games to display." << endl;
                    }
                    for (i = 0; i < num_games ; i++) {
                        cout << "Similarity: " << fixed << setprecision(4) << topSimilarGames.top().first << " | Game:  " << topSimilarGames.top().second << endl;
                        topSimilarGames.pop();
                    }
                    cout << "q - quit; m - print " << num_games << " more games; r - return to the main menu" << endl;
                    cin >> response;
                }
                break;

            default:
                cout << "Please enter a valid choice." << endl;
                break;
        }
        if (response == "r")
        {
            continue;
        }
        if (response == "q")
        {
            cout << "Thank you for using our program!\nCredits:\nBayan Mahmoodi\nKushagra Katiyar\nAgnivesh Kaundinya\nexiting..." << endl;
        }
        else
        {
            cout << "Invalid choice, exiting..." << endl;
            break;
        }
    return 0;
}