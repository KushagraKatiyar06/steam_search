//
// Created by kkati on 7/27/2025.
//

#include "readJson.h"

using json = nlohmann::json;
using namespace std;

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
