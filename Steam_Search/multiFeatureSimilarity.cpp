//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#include "multiFeatureSimilarity.h"
#include "Game.h"

#include <numeric>
#include <set>
#include <cmath> // For std::abs

// Calculates Weighted Jaccard Similarity for tags.
double calculateWeighted(
    const std::unordered_map<std::string, int>& tagsA,
    const std::unordered_map<std::string, int>& tagsB) {

    double intersectionSum = 0.0; // needed for formula
    double unionSum = 0.0; // second part of formula

    std::set<std::string> allUniqueTags; // get tags from both A and B
    for (const auto& pair : tagsA){
        allUniqueTags.insert(pair.first);
    }
    for (const auto& pair : tagsB){
        allUniqueTags.insert(pair.first);
    }

    for (const std::string& tag : allUniqueTags){ // calculate sums based on tag information
        int weightA = 0;
        auto itA = tagsA.find(tag);
        if (itA != tagsA.end()){weightA = itA->second;}

        int weightB = 0;
        auto itB = tagsB.find(tag);
        if (itB != tagsB.end()){weightB = itB->second;}

        intersectionSum += std::min(weightA, weightB);
        unionSum += std::max(weightA, weightB);
    }

    if (unionSum == 0.0){ // cannot divide by 0, thus this line
        return 0.0;
    }

    return intersectionSum / unionSum;
}

// Calculates standard Jaccard Similarity between two sets of strings.
double calculateJaccard(const std::set<std::string>& setA, const std::set<std::string>& setB) {

    int intersectionSize = 0; // simple intersection
    if (setA.size() < setB.size()){
        for (const auto& element : setA) {
            if (setB.count(element)) { // .count() is more efficient here
                intersectionSize++;
            }
        }
    }
    else {
        for (const auto& element : setB) {
            if (setA.count(element)) {
                intersectionSize++;
            }
        }
    }

    int unionSize = setA.size() + setB.size() - intersectionSize;

    if (unionSize == 0){ // division by zero avoidance
        return 0.0;
    }

    return static_cast<double>(intersectionSize) / unionSize;
}

// function to calculate similarity based on review score
double calculateReviewScoreSimilarity(double scoreA, double scoreB) {
    if (scoreA < 0 || scoreB < 0) { // If either score is -1.0 (no review score), return 0 similarity.
        return 0.0;
    }
    return 1.0 - std::abs(scoreA - scoreB);
}


// Calculates the overall weighted similarity between two games based on multiple features.
double calculateOverallWeightedSimilarity(
    const Game& gameA, // list of features
    const Game& gameB,
    double weightTags,
    double weightPublishers,
    double weightDevelopers,
    double weightReviewScore)
{

    double tagsSimilarity = calculateWeighted(gameA.getTags(), gameB.getTags()); // tags similarity

    double publishersSimilarity = 0.0; // Grab similarity for each feature
    std::set<std::string> publishersA_set;
    for (const auto& s : gameA.getPublisher()) {
        publishersA_set.insert(s);
    }
    std::set<std::string> publishersB_set;
    for (const auto& s : gameB.getPublisher()) {
        publishersB_set.insert(s);
    }

    publishersSimilarity = calculateJaccard(publishersA_set, publishersB_set);

    double developersSimilarity = 0.0; // Grab similarity for each feature
    std::set<std::string> developersA_set;
    for (const auto& s : gameA.getDevelopers()) {
        developersA_set.insert(s);
    }
    std::set<std::string> developersB_set;
    for (const auto& s : gameB.getDevelopers()) {
        developersB_set.insert(s);
    }

    developersSimilarity = calculateJaccard(developersA_set, developersB_set);

    double reviewScoreSimilarity = calculateReviewScoreSimilarity(gameA.getReviewScore(), gameB.getReviewScore()); // Grab similarity for each feature

    double totalWeight = weightTags + weightPublishers + weightDevelopers + weightReviewScore;
    if (totalWeight == 0.0) { // another divide by 0 error avoidance
        return 0.0;
    }

    double overallSimilarity = ( // overall similarity calculation
        (tagsSimilarity * weightTags) +
        (publishersSimilarity * weightPublishers) +
        (developersSimilarity * weightDevelopers) +
        (reviewScoreSimilarity * weightReviewScore)
    ) / totalWeight;

    return overallSimilarity;
}
