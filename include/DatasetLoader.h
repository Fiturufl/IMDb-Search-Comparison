#ifndef DATASET_LOADER_H
#define DATASET_LOADER_H

#include "Movie.h"

#include <string>
#include <vector>

class DatasetLoader {
public:
    static std::vector<Movie> loadMovies(
        const std::string& basicsFile
    );
};

#endif