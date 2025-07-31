#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <nlohmann/json.hpp>

#include "Game.h"
#include "readJson.h"
#include "minHash.h"
#include "jaccardsSimilarity.h"
#include "cosineSimilarity.h"


using json = nlohmann::json;
using namespace std;

// TODO: implement fuzzy matching or other normalizer so users can find games like The Crew™ 2. They would type The Crew 2 and be prompted "did you mean The Crew™ 2." These names have special characters normal users won't be able to input.
// also look into creating a drop down, as the user types in the name of their game the drop down gets shorter and shorter helping them hone in
// TODO: make names case insensitive and whitsepace insensitive
// TODO: string has issues with foreign characters, lines (3674, 43222, and more) of dataSet.txt good examples
// for example its known that ® and ™ don't appear properly. We can also outright remove these characters by manipulating the json.
// TODO: if a game has no tags make sure to use its genres instead (but we'll have no votes, set votes to -1)

// TODO: factor in the vote count of a tag wrt to the total votes of all tags for a game
// reasoning; don't want games punished for having users choose many tags but we still want the top tags to be most influential in the jaccards score

/*
double jaccards(string& a, string& b, unordered_map<string, Game>& gameData)
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
    };

    return static_cast<double>(intersection) / myUnion;
}

*/

/*
void readJson(json& dataJSON, unordered_map<string, Game>& allGames) {

    for (const auto& [game_id, game_info] : dataJSON.items()) {
        // iterates 111452 times (amount of games)
        string name = game_info.value("name", "");

        // TODO replace trash with more readable variable names

        allGames[name].setID(stoi(game_id));
        allGames[name].setPrice(static_cast<double>(game_info.value("price", 0.0)));
        allGames[name].setRequiredAge(game_info.value("requiredAge", 0));

        vector<string> platforms;
        if (game_info.value("windows", false) == true) {
            platforms.emplace_back("windows");
        }
        if (game_info.value("mac",false) == true) {
            platforms.emplace_back("mac");
        }
        if (game_info.value("linux",false) == true) {
            platforms.emplace_back("linux");
        }
        allGames[name].setPlatform(platforms);

        allGames[name].setMetacriticScore(game_info.value("metacritic_score", -1));

        vector<string> languages;
        for (auto& [trash1, trash] : game_info["supported_languages"].items()) {
            string language = to_string(trash).substr(1, to_string(trash).size()-2);
            languages.push_back(language);
        }
        allGames[name].setSupportedLanguages(languages);

        vector<string> audioLanguages;
        for (auto& [trash1, trash] : game_info["full_audio_languages"].items()) {
            string audio = to_string(trash).substr(1, to_string(trash).size()-2);
            audioLanguages.push_back(audio);
        }
        allGames[name].setFullAudioLanguages(audioLanguages);

        vector<string> developers;
        for (auto& [trash1, trash] : game_info["developers"].items()) {
            string developer = to_string(trash).substr(1, to_string(trash).size()-2);
            developers.push_back(developer);
        }
        allGames[name].setDevelopers(developers);

        vector<string> publishers;
        for (auto& [trash1, trash] : game_info["publishers"].items()) {
            string publisher = to_string(trash).substr(1, to_string(trash).size()-2);
            publishers.push_back(publisher);
        }
        allGames[name].setPublisher(publishers);

        vector<string> categories;
        for (auto& [trash1, trash] : game_info["categories"].items()) {
            string category = to_string(trash).substr(1, to_string(trash).size()-2);
            categories.push_back(category);
        }
        allGames[name].setCategories(categories);

        vector<string> genres;
        for (auto& [trash1, trash] : game_info["genres"].items()) {
            string genre = to_string(trash).substr(1, to_string(trash).size()-2);
            genres.push_back(genre);
        }
        allGames[name].setGenres(genres);

        unordered_map<string, int> tags;
        for (const auto& [tag_name, vote_count] : game_info["tags"].items()) {
            for (const auto& [trash2, trash] : vote_count.items()) {
                string votes = to_string(trash);
                if (votes == "0") {
                    continue;
                }
                tags[tag_name] = stoi(votes);
            }
        }
        allGames[name].setTags(tags);

        int positive = game_info.value("positive", 0);
        int negative = game_info.value("negative", 0);
        allGames[name].setReviewScore(positive, negative);
    }
}

*/
int main()
{

    cout << "Prepping dataset, and preprocessing data for algorithms" << endl;
    ifstream f("../steam_games.json"); // use ../steam_games_less.json for testing runs
    json dataJSON = json::parse(f);
    cout << dataJSON.size() << "\n" << endl;

    unordered_map<string, Game> metaData;
    readJson(dataJSON, metaData);

    string tagFile = "../tags.txt";
    unordered_map<string, int> indexedTags = readTags(tagFile);

    //minhashing preprep
    unordered_map<string, vector<int>> allSignatures;
    minHash minHash(150, indexedTags);

    for (const auto& pair : metaData) {
        const string& gameName = pair.first;
        const Game& game = pair.second;
        allSignatures[gameName] = minHash.createSignature(game);
    }

/*
    // collect metrics - completed, don't need to rerun
    unordered_map<string, string> decoder;
    for (const auto& [game_id, game_info] : dataJSON.items()) { // iterates 111452 times (amount of games)
        for (const auto& [game_name, trash] : game_info["name"].items()) {
            string name = to_string(trash).substr(1, to_string(trash).size()-2);
            decoder[game_id] = name;
        }
    }


    // save to file
    ofstream outFile("../decoder.txt");
    for (const auto& [key, value] : decoder) {
        outFile << key << '\t' << value << '\n';
    }
    */


    // update decoder from file
    unordered_map<string, string> decoder;
    ifstream inFile("../decoder.txt");
    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        if (string key, value; getline(iss, key, '\t') && getline(iss, value)) {
            decoder[key] = value;
        }
    }

    string game = "Need for Speed Unbound";

    // jaccards test code
    cout << "Jaccards comparison" << endl;

    string& source = game;
    string compare;
    priority_queue<pair<double, string>> maxHeap;
    for (const auto& [key, value] : decoder) {
        if (value != game) {
            compare = value;
        } else {
            continue;
        }
        maxHeap.emplace(jaccardsSimilarity(source, compare, metaData), value);
    }

    cout << "\nTop 10 most similar games:" << endl;
    cout << "--------------------------" << endl;
    for (int i = 0; i < 10 ; i++) {
        cout << "Similarity: " << maxHeap.top().first << " | Game:  " << maxHeap.top().second << endl;
        maxHeap.pop();
    }


    //minHash test code
    cout << "\nMin hash algorithm: " << endl;

    string& source2 = game;
    const vector<int>& sourceSignature = allSignatures[source2];
    priority_queue<pair<double,string>> similarGames;

    for (const auto& pair : allSignatures) {
        const string& compareGameName = pair.first;
        if (compareGameName == source2) {
            continue;
        }
        const vector<int>& compareSignature = pair.second;
        double similarity = minHash.miniJaccards(sourceSignature, compareSignature);
        similarGames.emplace(similarity, compareGameName);
    }

    cout << "\nTop 10 most similar games:" << endl;
    cout << "--------------------------" << endl;
    for (int i = 0; i < 10 && !similarGames.empty(); ++i) {
        pair<double, string> top = similarGames.top();
        cout << "Similarity: " << top.first << "  |  Game: " << top.second << endl;
        similarGames.pop();
    }

    // Cosine Similarity
    cout << "\nCosine Similarity algorithm: " << endl;

    cosineSimilarity cosineSim(indexedTags);
    cosineSim.createGameSignatures(metaData);
    priority_queue<pair<double, string>> cosineHeap;

    for (const auto& pair : metaData) {
        if (pair.first == game) {
            continue;
        }

        double similarity = cosineSim.similarity(game, pair.first);
        cosineHeap.emplace(similarity, pair.first);
    }

    cout << "\nTop 10 most similar games:" << endl;
    cout << "--------------------------" << endl;
    for (int i = 0; i < 10 && !cosineHeap.empty(); ++i) {
        pair<double, string> top = cosineHeap.top();
        cout << "Similarity: " << top.first << "  |  Game: " << top.second  << endl;
        cosineHeap.pop();
    }


    // pull tags from file
    /*set<string> tags;
    inFile.close();
    inFile.open("../tags.txt");
    line.clear();
    while (getline(inFile, line)) {
        tags.insert(line);
    }


    // populate the data map with empty sets
    unordered_map<string, unordered_map<string, string>> data;
    for (auto& element : tags) {
        data[element] = unordered_map<string, string>();
    }

    // populate the empty sets
    // TODO redo in the style of the readJson function
    for (const auto& [game_id, game_info] : dataJSON.items()) { // iterates 111452 times (amount of games)
        for (const auto& [tag_name, vote_count] : game_info["tags"].items()) {
            for (const auto& [trash2, trash] : vote_count.items()) {
                string votes = to_string(trash);
                if (votes == "0") {
                    continue;
                }
                data[tag_name][decoder[game_id]] = votes;
                // cout << "data[" << tag_name << "][" << decoder[game_id] << "]" << " = " << votes << endl;
            }
        }
    }*/

    // for (const auto& element : tags) {
    //     cout << "----------" << element << "----------" << endl;
    //     const auto& tag_data = data[element];
    //     for (const auto& [game, votes] : tag_data) {
    //         cout << game << ": " << votes << endl;
    //     }
    // }


    return 0;
}