//
// Created by kkati on 7/30/2025.
//

#include "cosineSimilarity.h"
#include <cmath>

//Constructor
cosineSimilarity:: cosineSimilarity(const unordered_map<string, int>& tags) {
    this->indexedTags = tags;

}

//Getters
unordered_map<string, int> cosineSimilarity::getIndexedTags() {
    return this->indexedTags;
}
unordered_map<string, vector<double>> cosineSimilarity::getGameSignatures() {
    return this->gameSignatures;
}

//Methods
void cosineSimilarity::createGameSignatures(const unordered_map<string, Game>& allData) {
    int numTags = indexedTags.size();

    for (auto const& game : allData) {
        string currentName = game.first;
        Game currentGame = game.second;
        vector<double> gameSignature(numTags, 0.0);

        int totalVotes = 0;
        for (auto tag : currentGame.getTags()) {
            totalVotes += tag.second;
        }

        for (auto tag : currentGame.getTags()) {

            string currentTag = tag.first;
            int currentVotes = tag.second;

            double score = static_cast<double>(currentVotes) / totalVotes;
            int newIndex = indexedTags[currentTag];

            gameSignature[newIndex] += score;
        }

        this->gameSignatures[currentName]= gameSignature;
    }

}

double cosineSimilarity:: similarity(const string& gameA, const string& gameB) {
    if (gameSignatures.find(gameA) == gameSignatures.end() || gameSignatures.find(gameB) == gameSignatures.end()) {
        return 0.0;
    }

    const vector<double>& gameSignatureA = gameSignatures[gameA];
    const vector<double>& gameSignatureB = gameSignatures[gameB];

    double dotProduct = 0.0;
    double normA = 0.0;
    double normB = 0.0;

    for (int i = 0; i < gameSignatureA.size(); i++) {
        dotProduct += gameSignatureA[i] * gameSignatureB[i];
        normA += pow(gameSignatureA[i], 2);
        normB += pow(gameSignatureB[i], 2);
    }

    normA = sqrt(normA);
    normB = sqrt(normB);

    if (normA == 0 || normB == 0) {
        return 0.0;
    }

    return dotProduct / (normA * normB);
}



