//
// Created by kkati on 7/27/2025.
//

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

using namespace std;


class Game {
private:

    //variables are in order based on order of the json
    int ID;
    int price;
    int requiredAge;

    vector<string> platforms;

    int metacriticScore;

    vector<string> supportedLanguages;
    vector<string> fullAudioLanguages;
    vector<string> developers;

    string publisher;

    vector<string> categories; //"Single-Player", "Multi-Player", etc.
    vector<string> genres; //"Casual", "Indie", etc

    int positive, negative; //specific review scores
    int reviewScore; //positive - negative

    vector<string> tags;

public:
    Game();

    //Getters
    int getID() const ;

    int getPrice() const;

    int getRequiredAge () const;

    vector<string> getPlatforms() const;

    int getMetacriticScore() const;

    vector<string> getSupportedLanguages() const;

    vector<string> getFullAudioLanguages() const;

    vector<string> getDevelopers() const;

    string getPublisher() const;

    vector<string> getCategories() const;

    vector<string> getGenres() const;

    int getPositive() const;

    int getNegative() const;

    int getReviewScore() const;//returns the actual review score accounting for downvotes and upvotes

    vector<string> getTags() const;

    //Setters
    void setID(int id);

    void setPrice(int price);

    void setRequiredAge(int age);

    void setPlatform(vector<string> platform);

    void setMetacriticScore(int score);

    void setSupportedLanguages(vector<string> supportedLanguages);

    void setFullAudioLanguages(vector<string> fullAudioLanguages);

    void setDevelopers(vector<string> developers);

    void setPublisher(string publisher);

    void setCategories(vector<string> categories);

    void setGenres(vector<string> genres);

    void setPositive(int positive);

    void setNegative(int negative);

    void setReviewScore(int score);

    void setTags(vector<string> tags);

    //methods
    int getTagCount();

};


#endif //GAME_H
