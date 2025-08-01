cmake_minimum_required(VERSION 3.29) # hey
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
                multiFeatureSimilarity.cpp
                multiFeatureSimilarity.h
                algorithms_B.cpp
                algorithms_B.h
                RapidFuzzie.cpp
                RapidFuzzie.h)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(Steam_Search PRIVATE nlohmann_json::nlohmann_json)

FetchContent_Declare(rapidfuzz GIT_REPOSITORY https://github.com/rapidfuzz/rapidfuzz-cpp.git GIT_TAG main)
FetchContent_MakeAvailable(rapidfuzz)

target_link_libraries(Steam_Search PRIVATE rapidfuzz::rapidfuzz)