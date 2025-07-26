#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main()
{
    std::ifstream f("steam_games.json");
    json data = json::parse(f);
    std::cout << data.size() << std::endl;
    return 0;
}
