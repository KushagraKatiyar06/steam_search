//
// Created by kkati on 7/27/2025.
//

#include "Game.h"

//Getters
int Game:: getID() const {
    return ID;
}

int Game:: getPrice() const {
    return price;
}

int Game:: getRequiredAge() const {
    return requiredAge;
}

vector<string> Game:: getPlatforms() const {
    return platforms;
}

int Game:: getMetacriticScore() const {
    return metacriticScore;
}

vector<string> Game :: getSupportedLanguages() const {
    return supportedLanguages;
}

vector<string> Game:: getDevelopers() const {
    return developers;
}

string Game:: getPublisher() const {
    return publisher;
}

vector<string> Game :: getCategories() const {
    return categories;
}

vector<string> Game:: getGenres() const {
    return genres;
}

int Game :: getPositive() const {
    return positive;
}

int Game :: getNegative() const {
    return negative;
}

int Game:: getReviewScore() const {
    return reviewScore;
}

vector<string> Game :: getTags() const {
    return tags;
}

//Setters
void Game :: setID(int id){
    this->ID = id;
}

void Game:: setPrice(int price) {
    this->price = price;
}

void Game:: setRequiredAge(int age) {
    this->requiredAge = age;
}

void Game:: setPlatform(vector<string> platform) {
    this->platforms = platform;
}

void Game :: setMetacriticScore(int score) {
    this->metacriticScore = score;
}

void Game :: setSupportedLanguages(vector<string> supportedLanguages) {
    this->supportedLanguages = supportedLanguages;
}

void Game :: setFullAudioLanguages(vector<string> fullAudioLanguages) {
    this->fullAudioLanguages = fullAudioLanguages;
}

void Game :: setDevelopers(vector<string> developers) {
    this->developers = developers;
}

void Game :: setPublisher(string publisher) {
    this->publisher = publisher;
}


void Game :: setCategories (vector<string> categories) {
    this->publisher = publisher;
}

void Game:: setGenres(vector<string> genres) {
    this->genres = genres;
}

void Game :: setPositive(int positive) {
    this->positive = positive;
}

void Game :: setNegative(int negative) {
    this->negative = negative;
}

void Game :: setReviewScore(int score) {
    this->reviewScore = score;
}

void Game :: setTags(vector<string> tags) {
    this->tags = tags;
}

//methods
int Game :: getTagCount() {
    return tags.size();
}



