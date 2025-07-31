//
// Created by Agnivesh Kaundinya on 7/31/2025.
//

#include "multiFeatureSimilarity.h"
#include "Game.h"

#include <numeric>
#include <set>

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

    if (unionSum == 0.0){return 0.0;}

    return intersectionSum / unionSum;
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

    if (unionSize == 0){return 0.0;}

    return static_cast<double>(intersectionSize) / unionSize;
}

double calculateOverallWeightedSimilarity(
    const Game& gameA,
    const Game& gameB,
    double weightTags,
    double weightPublishers,
    double weightDevelopers,
    double weightGenres) {

    double tagsSimilarity = calculateWeightedJaccard(gameA.getTags(), gameB.getTags());

    std::set<std::string> publishersA(gameA.getPublisher().begin(), gameA.getPublisher().end());
    std::set<std::string> publishersB(gameB.getPublisher().begin(), gameB.getPublisher().end());
    double publishersSimilarity = calculateJaccard(publishersA, publishersB);

    std::set<std::string> developersA(gameA.getDevelopers().begin(), gameA.getDevelopers().end());
    std::set<std::string> developersB(gameB.getDevelopers().begin(), gameB.getDevelopers().end());
    double developersSimilarity = calculateJaccard(developersA, developersB);

    std::set<std::string> genresA(gameA.getGenres().begin(), gameA.getGenres().end());
    std::set<std::string> genresB(gameB.getGenres().begin(), gameB.getGenres().end());
    double genresSimilarity = calculateJaccard(genresA, genresB);

    double totalWeight = weightTags + weightPublishers + weightDevelopers + weightGenres;

    if (totalWeight == 0.0) {return 0.0;}

    double overallSimilarity = (
        (tagsSimilarity * weightTags) +
        (publishersSimilarity * weightPublishers) +
        (developersSimilarity * weightDevelopers) +
        (genresSimilarity * weightGenres)
    ) / totalWeight;

    return overallSimilarity;
}
