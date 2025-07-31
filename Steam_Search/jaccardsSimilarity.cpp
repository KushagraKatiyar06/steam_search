//
// Created by kkati on 7/27/2025.
//

#include "jaccardsSimilarity.h"

// jaccard's tag similarity algorithm, calculates a similarity scores based on tag overlap between games
double jaccardsSimilarity(string& a, string& b, unordered_map<string, Game>& gameData)
{
    // collets both games tags, will be used to calculate set intersections and unions
    const auto& tagsA = gameData[a].getTags();
    const auto& tagsB = gameData[b].getTags();

    int intersection = 0;

    // calculate how many tags in common the games have (intersection)
    for (const auto& [tag, votes] : tagsA) {
        if (tagsB.contains(tag)) {
            intersection++;
        }
    }

    // get the total num of tags both games have
    int totalTagsA = gameData[a].getTagCount();
    int totalTagsB = gameData[b].getTagCount();

    // this caculates a union of the sets but adding the totals from other and removing the intersection
    // we're only concerned with sizes so these values are all just plain integers
    int myUnion = totalTagsA + totalTagsB - intersection;

    if (myUnion == 0) {
        return 0.0;
    }

    return static_cast<double>(intersection) / myUnion;
}

// same algo as above but takes into account what portion of the total votes those compared tags have for each game
double jaccardsSimilarityWeighted(string& a, string& b, unordered_map<string, Game>& gameData)
{
    // collets both games tags, will be used to calculate set intersections and unions
    const auto& tagsA = gameData[a].getTags();
    const auto& tagsB = gameData[b].getTags();

    // if one of the games doesn't have voted on tags run unweighted variant
    if (tagsA.empty() || tagsB.empty())
    {
        return jaccardsSimilarity(a, b, gameData);
    }

    double tagA_totalVotes = 0.0;
    double tagB_totalVotes = 0.0;

    // both of these loops determine the total amount of tag votes cast, will be used to normalize individual tag votes
    for (const auto& [tag, votes] : tagsA)
    {
        tagA_totalVotes += votes;
    }
    for (const auto& [tag, votes] : tagsB)
    {
        tagB_totalVotes += votes;
    }

    // here we caluclate the normalized tag votes, each tag now has its weighted votes clearly attached in a map 'weights'
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
        // for an individual tag checks both weights containers for the number of votes that tag got for each game
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