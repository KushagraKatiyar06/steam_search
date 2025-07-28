//
// Created by kkati on 7/27/2025.
//

#include "jaccardsSimilarity.h"

using namespace std;

double jaccardsSimilarity(string& a, string& b, unordered_map<string, Game>& gameData)
{
    const auto& tagsA = gameData[a].getTags();
    const auto& tagsB = gameData[b].getTags();

    int intersection = 0;

    for (const auto& [tag, votes] : tagsA) {
        if (tagsB.contains(tag)) {
            intersection++;
        }
    }

    int totalTagsA = gameData[a].getTagCount();
    int totalTagsB = gameData[b].getTagCount();

    int myUnion = totalTagsA + totalTagsB - intersection;

    if (myUnion == 0) {
        return 0.0;
    };

    return static_cast<double>(intersection) / myUnion;
}

