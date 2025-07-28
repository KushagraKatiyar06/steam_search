//
// Created by kkati on 7/27/2025.
//

#ifndef READJSON_H
#define READJSON_H

#include <string>
#include <unordered_map>

#include "Game.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void readJson(json& dataJSON, std::unordered_map<std::string, Game>& allGames);

#endif //READJSON_H
