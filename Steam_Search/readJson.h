//
// Created by kkati on 7/27/2025.
//

#ifndef READJSON_H
#define READJSON_H

#include <string>
#include <unordered_map>
#include <fstream>

#include "Game.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void readJson(json& dataJSON, std::unordered_map<std::string, Game>& allGames); //reads the json files and filled in every attribute for the Game objects.
void collectMetrics(json& dataJSON, unordered_map<string, string>& decoder); //
void saveToFile(unordered_map<string,string>& decoder);

unordered_map<string, int> readTags(const string& file);

#endif //READJSON_H
