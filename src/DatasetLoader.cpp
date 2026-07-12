#include "DatasetLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::vector<Movie> DatasetLoader::loadMovies(
    const std::string& basicsFile
) {
    std::vector<Movie> movies;

    std::ifstream basicsStream(basicsFile);

    if (!basicsStream.is_open()) {
        std::cerr << "Could not open basics file: "
                  << basicsFile << '\n';
        return movies;
    }

    std::string line;

    // Skip the header row.
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

        movies.push_back({
            id,
            primaryTitle,
            year,
            0.0f
        });
    }

    return movies;
}