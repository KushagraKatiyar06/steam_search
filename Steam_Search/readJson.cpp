//
// Created by kkati on 7/27/2025.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "Game.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

void readJson(json& dataJSON, unordered_map<string, Game>& allGames) {

    for (const auto& [game_id, game_info] : dataJSON.items()) { // iterates 111452 times (amount of games)
        for (const auto& [game_name, trash] : game_info["name"].items()) {
            allGames[game_name].setID(stoi(game_id));

            cout << trash << endl;
        }
    }

}
