//
// Created by bmmah on 7/30/2025.
//

#include "algorithms_B.h"

#include <iostream>
#include <ostream>

// this function takes in two games (selected and candidate) and then decides what bucket to place candidate in wrt to selected
BucketLevel setBucket(string& selected, string& candidate, unordered_map<string, Game>& gameData, unordered_map<string, double> scores)
{
    bool sameDev = false;
    // checks if the selected game has the same publisher as the candidate game, need to iterate through containers
    // of devs and publishers because some games have more than one
    for (const auto& srcDevs : gameData[selected].getDevelopers())
    {
        for(const auto& otherDevs : gameData[candidate].getDevelopers())
        {
            if (otherDevs == srcDevs)
            {
                sameDev = true;
                break;
            }
        }
        if (sameDev)
        {
            break;
        }
    }

    bool samePub = false;
    // this is the publisher check code, the for loops above check for dev overlap
    for (const auto& srcPubs : gameData[selected].getPublisher())
    {
        for(const auto& otherPubs : gameData[candidate].getPublisher())
        {
            if (otherPubs == srcPubs)
            {
                samePub = true;
                break;
            }
        }
        if (samePub)
        {
            break;
        }
    }

    double review = gameData[candidate].getReviewScore();

    // used an enum to simplify the output process, don't need some abstract values or other method
    if (sameDev) return HIGH_RELEVANCE;
    if (samePub) return MEDIUM_RELEVANCE;
    if (scores[candidate] >= 0.75) return TAG_SIMILAR;
    if (scores[candidate] >= 0.25 && review > 85) return WEAK_SIMILAR;
    return LOW_RELEVANCE;
}

// this function embodies the entire rule-based decision tree logic
vector<string> decisionTree(string& selected, unordered_map<string, Game>& gameData, unordered_map<string, string> decoder, int num_games)
{
    map<BucketLevel, vector<string>> buckets;
    unordered_map<string, int> tags = gameData[selected].getTags();
    unordered_set<string> candidates;

    // this section creates a maxHeap of games (uses weighted jaccards) which will be the similarity score that was used
    // in setBucket (scores[candidate] >= 0.75) return TAG_SIMILAR
    string source = selected;
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
        maxHeap.emplace(jaccardsSimilarityWeighted(source, compare, gameData), value);
    }

    // here we're finding the maximum of the jaccards scores so we can normalize maxHeap before calling setBucket
    unordered_map<string, double> scores;
    double maxScore = 0.0;
    int count = 0;
    while (!maxHeap.empty() && count < num_games) {
        auto [sim, candidate] = maxHeap.top();
        maxHeap.pop();
        if (candidate == selected)
        {
            continue;
        }
        scores[candidate] = sim;
        maxScore = max(maxScore, sim);
        count++;
    }

    // now normalize the maxHeap (which has been transferred to scores container)
    for (auto& [name, score] : scores) {
        if (maxScore > 0)
            score /= maxScore; // scale all scores to [0, 1]
    }

    // find all games that share a tag with the selected game, these will be our candidates for the decision tree
    for (auto& [game, data] : gameData)
    {
        for (auto& [tag, votes] : data.getTags())
        {
            if (tags.contains(tag) && game != selected)
            {
                candidates.insert(game);
                break;
            }
        }
    }

    // iterate through the candidates container and for each candidate decide which bucket it belongs in
    for (auto& candidate : candidates)
    {
        string name = candidate;
        BucketLevel level = setBucket(selected, name, gameData, scores);
        buckets[level].push_back(name);
    }

    // buckets is a 2D container, one dimension is the enum from above (buckets[enum]) and the other dimension
    // is all the games that belong in that enum's bucket category (high-relevance, etc). This loop sorts each
    // bucket by review score
    for (auto& [bucket, games] : buckets)
    {
        sort(games.begin(), games.end(), [&gameData](const string& a, const string& b)
        {
            return gameData[a].getReviewScore() > gameData[b].getReviewScore();
        });
    }

    vector<string> finalResults;
    // here we're accessing the buckets container and returning its contents, we use the enums to simplify traversal
    for (int level = HIGH_RELEVANCE; level <= LOW_RELEVANCE; level++)
    {
        const auto& bucket = buckets[static_cast<BucketLevel>(level)];
        finalResults.insert(finalResults.end(), bucket.begin(), bucket.end());
    }
    return finalResults;
}