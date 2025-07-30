//
// Created by kkati on 7/27/2025.
//

#ifndef JACCARDSSIMILARITY_H
#define JACCARDSSIMILARITY_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Game.h"

using namespace std;

double jaccardsSimilarity(string& a, string& b, unordered_map<string, Game>& gameData);
double jaccardsSimilarityWeighted(string& a, string& b, unordered_map<string, Game>& gameData);



#endif //JACCARDSSIMILARITY_H
