#include "DatasetLoader.h"
#include "RedBlackTree.h"

#include <chrono>
#include <iostream>
#include <vector>

int main() {
    const std::string basicsFile = "data/title.basics.tsv";
    const std::string ratingsFile = "data/title.ratings.tsv";

    std::cout << "Loading IMDb dataset...\n";

    std::vector<Movie> movies =
        DatasetLoader::loadMovies(basicsFile, ratingsFile);

    if (movies.empty()) {
        std::cerr << "No movies loaded.\n";
        return 1;
    }

    std::cout << "Loaded " << movies.size() << " movies.\n";

    RedBlackTree tree;

    auto insertStart = std::chrono::high_resolution_clock::now();

    for (const Movie& movie : movies) {
        tree.insert(movie);
    }

    auto insertEnd = std::chrono::high_resolution_clock::now();

    auto insertMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            insertEnd - insertStart
        ).count();

    std::cout << "Insertion time: " << insertMs << " ms\n";

    const std::string searchId = movies[movies.size() / 2].id;

    auto searchStart = std::chrono::high_resolution_clock::now();

    Movie* result = tree.search(searchId);

    auto searchEnd = std::chrono::high_resolution_clock::now();

    auto searchNs =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            searchEnd - searchStart
        ).count();

    std::cout << "Search time: " << searchNs << " ns\n";

    if (result != nullptr) {
        std::cout << "Found: " << result->title
                  << " (" << result->year << ")\n";
    }

    auto deleteStart = std::chrono::high_resolution_clock::now();

    bool removed = tree.remove(searchId);

    auto deleteEnd = std::chrono::high_resolution_clock::now();

    auto deleteNs =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            deleteEnd - deleteStart
        ).count();

    std::cout << "Deletion time: " << deleteNs << " ns\n";
    std::cout << (removed ? "Deleted successfully\n" : "Deletion failed\n");

    return 0;
}