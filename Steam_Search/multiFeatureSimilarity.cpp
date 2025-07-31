//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#include "multiFeatureSimilarity.h"
#include "Game.h"

#include <numeric>
#include <set>
#include <cmath> // For std::abs

double calculateWeightedJaccard(
    const std::unordered_map<std::string, int>& tagsA,
    const std::unordered_map<std::string, int>& tagsB) {

    double intersectionSum = 0.0;
    double unionSum = 0.0;

    std::set<std::string> allUniqueTags;
    for (const auto& pair : tagsA){
        allUniqueTags.insert(pair.first);
    }
    for (const auto& pair : tagsB){
        allUniqueTags.insert(pair.first);
    }

    for (const std::string& tag : allUniqueTags){
        int weightA = 0;
        auto itA = tagsA.find(tag);
        if (itA != tagsA.end()){weightA = itA->second;}

        int weightB = 0;
        auto itB = tagsB.find(tag);
        if (itB != tagsB.end()){weightB = itB->second;}

        intersectionSum += std::min(weightA, weightB);
        unionSum += std::max(weightA, weightB);
    }

    if (unionSum == 0.0){
        return 0.0;
    }

    double result = intersectionSum / unionSum;
    return result;
}

double calculateJaccard(
    const std::set<std::string>& setA,
    const std::set<std::string>& setB) {

    int intersectionSize = 0;
    if (setA.size() < setB.size()){
        for (const std::string& item : setA) {
            if (setB.count(item)){intersectionSize++;}
        }
    }
    else {
        for (const std::string& item : setB) {
            if (setA.count(item)){intersectionSize++;}
        }
    }

    int unionSize = setA.size() + setB.size() - intersectionSize;

    if (unionSize == 0){
        return 0.0;
    }

    double result = static_cast<double>(intersectionSize) / unionSize;
    return result;
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
    return 1.0 - std::abs(scoreA - scoreB);
}


double calculateOverallWeightedSimilarity(
    const Game& gameA,
    const Game& gameB,
    double weightTags,
    double weightPublishers,
    double weightDevelopers,
    double weightReviewScore) { // Changed from weightGenres to weightReviewScore

    double tagsSimilarity = calculateWeightedJaccard(gameA.getTags(), gameB.getTags());

    double publishersSimilarity = 0.0;
    std::set<std::string> publishersA_set;
    for (const auto& s : gameA.getPublisher()) {
        publishersA_set.insert(s);
    }
    std::set<std::string> publishersB_set;
    for (const auto& s : gameB.getPublisher()) {
        publishersB_set.insert(s);
    }
    publishersSimilarity = calculateJaccard(publishersA_set, publishersB_set);

    double developersSimilarity = 0.0;
    std::set<std::string> developersA_set;
    for (const auto& s : gameA.getDevelopers()) {
        developersA_set.insert(s);
    }
    std::set<std::string> developersB_set;
    for (const auto& s : gameB.getDevelopers()) {
        developersB_set.insert(s);
    }
    developersSimilarity = calculateJaccard(developersA_set, developersB_set);

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


// hey