//
// Created by kkati on 7/27/2025.
//

#include "jaccardsSimilarity.h"


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
    }

    return static_cast<double>(intersection) / myUnion;
}

double jaccardsSimilarityWeighted(string& a, string& b, unordered_map<string, Game>& gameData)
{
    const auto& tagsA = gameData[a].getTags();
    const auto& tagsB = gameData[b].getTags();

    // if one of the games doesn't have voted on tags run unweighted variant
    if (tagsA.empty() || tagsB.empty())
    {
        return jaccardsSimilarity(a, b, gameData);
    }

    double tagA_totalVotes = 0.0;
    double tagB_totalVotes = 0.0;

    for (const auto& [tag, votes] : tagsA)
    {
        tagA_totalVotes += votes;
    }

    for (const auto& [tag, votes] : tagsB)
    {
        tagB_totalVotes += votes;
    }

    unordered_map<string, double> weightsA, weightsB;

    for (const auto& [tag, votes] : tagsA)
    {
        weightsA[tag] = votes / tagA_totalVotes;
    }
    for (const auto& [tag, votes] : tagsB)
    {
        weightsB[tag] = votes / tagB_totalVotes;
    }

    double intersection = 0.0;
    double unionSum = 0.0;

    // Compute weighted Jaccard
    unordered_set<string> allTags;
    for (const auto& [tag, weight] : weightsA) allTags.insert(tag);
    for (const auto& [tag, weight] : weightsB) allTags.insert(tag);

    for (const auto& tag : allTags)
    {
        double wa = weightsA.count(tag) ? weightsA[tag] : 0.0;
        double wb = weightsB.count(tag) ? weightsB[tag] : 0.0;

        intersection += std::min(wa, wb);
        unionSum += std::max(wa, wb);
    }

    if (unionSum == 0.0)
    {
        return 0.0;
    }
    return intersection / unionSum;
}