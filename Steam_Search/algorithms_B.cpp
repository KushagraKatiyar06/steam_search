//
// Created by bmmah on 7/30/2025.
//

#include "algorithms_B.h"

#include <iostream>
#include <ostream>

BucketLevel setBucket(string& selected, string& candidate, unordered_map<string, Game>& gameData, unordered_map<string, double> scores)
{
    // If same developer → priority bucket 1
    // Else if ≥80 % tag overlap → priority 2
    // Else if ≥60 % tag overlap + ≥ 85 % review match → priority 3
    // Within each bucket, sort by review score

    bool sameDev = false;
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

    if (sameDev) return HIGH_RELEVANCE;
    if (samePub) return MEDIUM_RELEVANCE;
    if (scores[candidate] >= 0.75) return TAG_SIMILAR;
    if (scores[candidate] >= 0.25 && review > 85) return WEAK_SIMILAR;
    return LOW_RELEVANCE;
}

vector<string> decisionTree(string& selected, unordered_map<string, Game>& gameData, unordered_map<string, string> decoder)
{


    map<BucketLevel, vector<string>> buckets;
    unordered_map<string, int> tags = gameData[selected].getTags();
    unordered_set<string> candidates;

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

    // Compute all tag similarity scores, change the count limit in order to take into account more games
    unordered_map<string, double> scores;
    double maxScore = 0.0;
    int count = 0;
    while (!maxHeap.empty() && count < 10) {
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

    // Normalize
    for (auto& [name, score] : scores) {
        if (maxScore > 0)
            score /= maxScore; // scale all scores to [0, 1]
    }

    // find all games that share a tag with our selected game
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

    for (auto& candidate : candidates)
    {
        string name = candidate;
        BucketLevel level = setBucket(selected, name, gameData, scores);
        buckets[level].push_back(name);
    }

    for (auto& [bucket, games] : buckets)
    {
        sort(games.begin(), games.end(), [&gameData](const string& a, const string& b)
        {
            return gameData[a].getReviewScore() > gameData[b].getReviewScore();
        });
    }

    vector<string> finalResults;
    for (int level = HIGH_RELEVANCE; level <= LOW_RELEVANCE; level++)
    {
        const auto& bucket = buckets[static_cast<BucketLevel>(level)];
        finalResults.insert(finalResults.end(), bucket.begin(), bucket.end());
    }
    return finalResults;
}