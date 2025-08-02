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


    //iterate thorugh every game in the dataset nd initialize vectors of size numTags with value 0.
    //Then every game will be traversed through. Total votes across all tags is computed and in the vector,
    //each index represents a specific tag. The value at the index is
    //(number of votes for particular tag) / (total votes for every tag the game has).
    for (auto const& game : allData) {
        string currentName = game.first;
        Game currentGame = game.second;
        vector<double> gameSignature(numTags, 0.0);


        //Sums of all the votes for every tag the current game has accumulated
        int totalVotes = 0;
        for (auto tag : currentGame.getTags()) {
            totalVotes += tag.second;
        }

        //Populating the vector with the proportion of votes that a particular tag has relative to all
        //votes for every tag in the game
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

//Now two games have their own signatures which is vector full of the weighted voting each tag has. The dot product
//of both signatures provides the similiarity score after it is normalized using the magnitude of the vector ps: i learned this
//in linear algebra 2 days before writing this algorithm ;P.
double cosineSimilarity:: similarity(const string& gameA, const string& gameB) {
    if (gameSignatures.find(gameA) == gameSignatures.end() || gameSignatures.find(gameB) == gameSignatures.end()) {
        return 0.0;
    }

    const vector<double>& gameSignatureA = gameSignatures[gameA];
    const vector<double>& gameSignatureB = gameSignatures[gameB];

    double dotProduct = 0.0;
    double normA = 0.0;
    double normB = 0.0;

    //Calculates the dot product of the two vectors
    for (int i = 0; i < gameSignatureA.size(); i++) {
        dotProduct += gameSignatureA[i] * gameSignatureB[i];
        normA += pow(gameSignatureA[i], 2);
        normB += pow(gameSignatureB[i], 2);
    }

    //normA squares the signatures of the game to help with normalizing the data
    //square rooting the previously squared normal value. The normalizing ensures that the score
    //reamins in between 0 and 1.
    normA = sqrt(normA);
    normB = sqrt(normB);

    if (normA == 0 || normB == 0) {
        return 0.0;
    }

    return dotProduct / (normA * normB);
}



