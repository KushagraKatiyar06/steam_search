#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

struct GameData {
    string id;
    string name;
    int age;
    int price;
    vector<int> platform; // mac, linux, windows
    int metaScore; // metacritic
    vector<string> supportedLanguages;
    vector<string> fullAudioLanguages;
    vector<string> developer;
    string publisher;
    vector<string> categories; // singeplayer, multiplayer, etc
    int reviewScore; // positive/(positive+negative)
    vector<pair<string, int>> tags; // ints are the number of

}; // map<int, string> int == game_id, string == name

// TODO: implement fuzzy matching or other normalizer so users can find games like The Crew™ 2. They would type The Crew 2 and be prompted "did you mean The Crew™ 2." These names have special characters normal users won't be able to input.
// TODO: string has issues with foreign characters, lines (3674, 43222, and more) of dataSet.txt good examples

int main()
{
    ifstream f("../steam_games.json"); // use ../steam_games_less.json for testing runs
    json dataJSON = json::parse(f);
    cout << dataJSON.size() << endl;

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

    // pull tags from file
    set<string> tags;
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
    }

    for (const auto& element : tags) {
        cout << "----------" << element << "----------" << endl;
        const auto& tag_data = data[element];
        for (const auto& [game, votes] : tag_data) {
            cout << game << ": " << votes << endl;
        }
    }


    return 0;
}