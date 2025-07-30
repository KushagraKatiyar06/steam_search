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

// TODO: if a game has no tags make sure to use its genres instead (but we'll have no votes, set votes to -1)

// TODO: factor in the vote count of a tag wrt to the total votes of all tags for a game
// reasoning; don't want games punished for having users choose many tags but we still want the top tags to be most influential in the jaccards score

int main()
{
    ifstream f("../steam_games.json"); // use ../steam_games_less.json for testing runs
    json dataJSON = json::parse(f);
    cout << dataJSON.size() << endl;

    unordered_map<string, Game> metaData;

    readJson(dataJSON, metaData);

    /*// collect metrics - completed, don't need to rerun
    unordered_map<string, string> decoder;
    for (const auto& [game_id, game_info] : dataJSON.items()) { // iterates 111452 times (amount of games)
        for (const auto& [game_name, trash] : game_info["name"].items()) {
            string name = to_string(trash).substr(1, to_string(trash).size()-2);
            decoder[game_id] = name;
        }
    }

    // save to file
    ofstream outFile("../decoder.txt");
    for (const auto& [key, value] : decoder) {
        outFile << key << '\t' << value << '\n';
    }*/

    // update decoder from file
    unordered_map<string, string> decoder;
    ifstream inFile("../decoder.txt");
    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        if (string key, value; getline(iss, key, '\t') && getline(iss, value)) {
            decoder[key] = value;
        }
    }


    // decision tree test code
    string source = "Stellaris";
    vector<string> rankings = decisionTree(source, metaData, decoder);
    for (int i = 0; i < 10; i++)
    {
        cout << rankings[i] << endl;
    }

    // jaccards test code
    /*
    string source = "Shadows of Forbidden Gods";
    string compare;
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
    for (int i = 0; i < 10 ; i++) {
        cout << maxHeap.top().first << " : " << maxHeap.top().second << endl;
        maxHeap.pop();
    }*/


    // pull tags from file
    /*set<string> tags;
    inFile.close();
    inFile.open("../tags.txt");
    line.clear();
    while (getline(inFile, line)) {
        tags.insert(line);
    }


    // populate the data map with empty sets
    unordered_map<string, unordered_map<string, string>> data;
    for (auto& element : tags) {
        data[element] = unordered_map<string, string>();
    }

    // populate the empty sets
    // TODO redo in the style of the readJson function
    for (const auto& [game_id, game_info] : dataJSON.items()) { // iterates 111452 times (amount of games)
        for (const auto& [tag_name, vote_count] : game_info["tags"].items()) {
            for (const auto& [trash2, trash] : vote_count.items()) {
                string votes = to_string(trash);
                if (votes == "0") {
                    continue;
                }
                data[tag_name][decoder[game_id]] = votes;
                // cout << "data[" << tag_name << "][" << decoder[game_id] << "]" << " = " << votes << endl;
            }
        }
    }*/

    // for (const auto& element : tags) {
    //     cout << "----------" << element << "----------" << endl;
    //     const auto& tag_data = data[element];
    //     for (const auto& [game, votes] : tag_data) {
    //         cout << game << ": " << votes << endl;
    //     }
    // }


    return 0;
}