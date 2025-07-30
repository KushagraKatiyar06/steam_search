cmake_minimum_required(VERSION 3.29)
project(Steam_Search)

set(CMAKE_CXX_STANDARD 20)

add_executable(Steam_Search
                main.cpp
                Game.h
                Game.cpp
                readJson.h
                readJson.cpp
                jaccardsSimilarity.cpp
                jaccardsSimilarity.h
                minHash.cpp
                minHash.h
        cosineSimilarity.cpp
        cosineSimilarity.h
)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(Steam_Search PRIVATE nlohmann_json::nlohmann_json)