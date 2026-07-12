#include "DatasetLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

std::vector<Movie> DatasetLoader::loadMovies(
    const std::string& basicsFile,
    const std::string& ratingsFile
) {
    std::unordered_map<std::string, float> ratings;
    std::vector<Movie> movies;

    std::ifstream ratingsStream(ratingsFile);

    if (!ratingsStream.is_open()) {
        std::cerr << "Could not open ratings file: "
                  << ratingsFile << '\n';
        return movies;
    }

    std::string line;

    // Skip ratings header.
    std::getline(ratingsStream, line);

    while (std::getline(ratingsStream, line)) {
        std::stringstream stream(line);

        std::string id;
        std::string ratingText;
        std::string voteCount;

        std::getline(stream, id, '\t');
        std::getline(stream, ratingText, '\t');
        std::getline(stream, voteCount, '\t');

        try {
            ratings[id] = std::stof(ratingText);
        } catch (...) {
            // Skip malformed records.
        }
    }

    std::ifstream basicsStream(basicsFile);

    if (!basicsStream.is_open()) {
        std::cerr << "Could not open basics file: "
                  << basicsFile << '\n';
        return movies;
    }

    // Skip basics header.
    std::getline(basicsStream, line);

    while (std::getline(basicsStream, line)) {
        std::stringstream stream(line);

        std::string id;
        std::string titleType;
        std::string primaryTitle;
        std::string originalTitle;
        std::string isAdult;
        std::string startYear;
        std::string endYear;
        std::string runtimeMinutes;
        std::string genres;

        std::getline(stream, id, '\t');
        std::getline(stream, titleType, '\t');
        std::getline(stream, primaryTitle, '\t');
        std::getline(stream, originalTitle, '\t');
        std::getline(stream, isAdult, '\t');
        std::getline(stream, startYear, '\t');
        std::getline(stream, endYear, '\t');
        std::getline(stream, runtimeMinutes, '\t');
        std::getline(stream, genres, '\t');

        if (titleType != "movie" || startYear == "\\N") {
            continue;
        }

        int year;

        try {
            year = std::stoi(startYear);
        } catch (...) {
            continue;
        }

        float rating = 0.0f;

        auto ratingResult = ratings.find(id);

        if (ratingResult != ratings.end()) {
            rating = ratingResult->second;
        }

        movies.emplace_back(id, primaryTitle, year, rating);
    }

    return movies;
}
