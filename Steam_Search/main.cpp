#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <nlohmann/json.hpp>
#include "Game.h"

#include "readJson.h"
#include "jaccardsSimilarity.h"
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
    ifstream f("../steam_games_less.json"); // use ../steam_games_less.json for testing runs
    json dataJSON = json::parse(f);
    cout << "done parsing JSON" << endl;
    string source;

    unordered_map<string, Game> metaData;

    readJson(dataJSON, metaData);

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
    }*/

    return 0;
}