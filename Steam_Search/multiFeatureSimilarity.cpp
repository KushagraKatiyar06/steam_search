//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#include "multiFeatureSimilarity.h"
#include "Game.h"

#include <numeric>
#include <set>
#include <cmath>

double jaccardsSimilarityModified(const set<string>& setA, const set<string>& setB) {

    int intersection = 0;
    // calculate how many tags in common the games have (intersection)
    for (const auto& element : setA) {
        if (setB.contains(element)) {
            intersection++;
        }
    }

    int myUnion = setA.size() + setB.size() - intersection;

    if (myUnion == 0){
        return 0.0;
    }

    return static_cast<double>(intersection) / myUnion;
}

// function to calculate similarity based on review score
// Review scores are typically normalized between 0 and 1.
// Similarity is 1.0 - absolute difference.
double calculateReviewScoreSimilarity(double scoreA, double scoreB) {
    // If either score is -1.0 (indicating no review score), treat as 0 similarity to avoid
    // comparing an unknown score, or handle as per specific business logic.
    // For simplicity, if either is -1.0, we'll return 0.0 similarity for this feature.
    if (scoreA < 0 || scoreB < 0) {
        return 0.0;
    }
    // Scores are assumed to be normalized between 0.0 and 1.0
    return 1.0 - abs(scoreA - scoreB);
}


double calculateOverallWeightedSimilarity(const Game& gameA, const Game& gameB, double weightTags, double weightPublishers, double weightDevelopers, double weightReviewScore, unordered_map<string, Game>& gameData, unordered_map<string, string> decoder)
{

    double tagsSimilarity = jaccardsSimilarityWeighted(decoder[to_string(gameA.getID())], decoder[to_string(gameA.getID())], gameData);

    double publishersSimilarity = 0.0;
    set<string> publishersA_set;
    for (const auto& s : gameA.getPublisher()) {
        publishersA_set.insert(s);
    }
    set<string> publishersB_set;
    for (const auto& s : gameB.getPublisher()) {
        publishersB_set.insert(s);
    }
    publishersSimilarity = jaccardsSimilarityModified(publishersA_set, publishersB_set);

    double developersSimilarity = 0.0;
    set<string> developersA_set;
    for (const auto& s : gameA.getDevelopers()) {
        developersA_set.insert(s);
    }
    set<string> developersB_set;
    for (const auto& s : gameB.getDevelopers()) {
        developersB_set.insert(s);
    }
    developersSimilarity = jaccardsSimilarityModified(developersA_set, developersB_set);

    // Calculate Review Score Similarity instead of Genres Similarity
    double reviewScoreSimilarity = calculateReviewScoreSimilarity(gameA.getReviewScore(), gameB.getReviewScore());

    // Update totalWeight to include review score weight and exclude genre weight
    double totalWeight = weightTags + weightPublishers + weightDevelopers + weightReviewScore;
    if (totalWeight == 0.0) {
        return 0.0;
    }

    double overallSimilarity = (
        (tagsSimilarity * weightTags) +
        (publishersSimilarity * weightPublishers) +
        (developersSimilarity * weightDevelopers) +
        (reviewScoreSimilarity * weightReviewScore) // Use reviewScoreSimilarity
    ) / totalWeight;

    return overallSimilarity;
}