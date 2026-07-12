#include "DatasetLoader.h"
#include "MovieHashTable.h"
#include "RedBlackTree.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

std::string toLower(std::string text) {
    std::transform(
        text.begin(),
        text.end(),
        text.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::tolower(character));
        }
    );

    return text;
}

void searchByTitle(
    const std::vector<Movie>& movies,
    const std::string& searchText
) {
    std::string lowerSearch = toLower(searchText);
    int matchCount = 0;
    const int displayLimit = 20;

    auto start = Clock::now();

    for (const Movie& movie : movies) {
        if (toLower(movie.title).find(lowerSearch) != std::string::npos) {
            if (matchCount < displayLimit) {
                std::cout << movie.id
                          << " | "
                          << movie.title
                          << " | "
                          << movie.year
                          << '\n';
            }

            ++matchCount;
        }
    }

    auto end = Clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start
        ).count();

    std::cout << "\nMatches found: " << matchCount << '\n';
    std::cout << "Title search time: " << elapsed << " ms\n";

    if (matchCount > displayLimit) {
        std::cout << "Only the first "
                  << displayLimit
                  << " results are displayed.\n";
    }
}

int main() {
    const std::string basicsFile = "data/title.basics.tsv";

    std::cout << "Loading IMDb dataset...\n";

    std::vector<Movie> movies =
        DatasetLoader::loadMovies(basicsFile);

    if (movies.empty()) {
        std::cerr << "No movies loaded.\n";
        return 1;
    }

    std::cout << "Loaded "
              << movies.size()
              << " movies.\n";

    MovieHashTable hashTable;
    RedBlackTree tree;

    auto hashInsertStart = Clock::now();

    for (const Movie& movie : movies) {
        hashTable.insert(movie);
    }

    auto hashInsertEnd = Clock::now();

    auto treeInsertStart = Clock::now();

    for (const Movie& movie : movies) {
        tree.insert(movie);
    }

    auto treeInsertEnd = Clock::now();

    long long hashInsertionTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            hashInsertEnd - hashInsertStart
        ).count();

    long long treeInsertionTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            treeInsertEnd - treeInsertStart
        ).count();

    std::cout << "Data structures are ready.\n";

    while (true) {
        std::cout
            << "\n====================================\n"
            << "       IMDb Movie Search Menu\n"
            << "====================================\n"
            << "1. Search movie by title\n"
            << "2. Search movie by ID\n"
            << "3. Insert movie\n"
            << "4. Delete movie\n"
            << "5. Show performance results\n"
            << "6. Exit\n"
            << "Enter choice: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::string title;

            std::cout << "Enter a movie title or part of a title: ";
            std::getline(std::cin, title);

            if (title.empty()) {
                std::cout << "Title cannot be empty.\n";
                continue;
            }

            searchByTitle(movies, title);
        }

        else if (choice == "2") {
            std::string id;

            std::cout << "Enter IMDb ID, such as tt0111161: ";
            std::getline(std::cin, id);

            auto hashStart = Clock::now();
            Movie* hashResult = hashTable.search(id);
            auto hashEnd = Clock::now();

            auto treeStart = Clock::now();
            Movie* treeResult = tree.search(id);
            auto treeEnd = Clock::now();

            long long hashTime =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    hashEnd - hashStart
                ).count();

            long long treeTime =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    treeEnd - treeStart
                ).count();

            if (hashResult != nullptr) {
                std::cout << "\nMovie found:\n"
                          << "ID: " << hashResult->id << '\n'
                          << "Title: " << hashResult->title << '\n'
                          << "Year: " << hashResult->year << '\n';
            } else {
                std::cout << "Movie not found.\n";
            }

            std::cout << "\nSearch performance:\n"
                      << "Hash Table: "
                      << hashTime
                      << " ns\n"
                      << "Red-Black Tree: "
                      << treeTime
                      << " ns\n";

            if ((hashResult == nullptr) != (treeResult == nullptr)) {
                std::cout
                    << "Warning: The structures returned different results.\n";
            }
        }

        else if (choice == "3") {
            std::string id;
            std::string title;
            std::string yearText;

            std::cout << "Enter IMDb ID: ";
            std::getline(std::cin, id);

            std::cout << "Enter movie title: ";
            std::getline(std::cin, title);

            std::cout << "Enter release year: ";
            std::getline(std::cin, yearText);

            int year;

            try {
                year = std::stoi(yearText);
            } catch (...) {
                std::cout << "Invalid year.\n";
                continue;
            }

            if (id.empty() || title.empty()) {
                std::cout << "ID and title cannot be empty.\n";
                continue;
            }

            Movie movie{
                id,
                title,
                year,
                0.0f
            };

            auto hashStart = Clock::now();
            bool hashInserted = hashTable.insert(movie);
            auto hashEnd = Clock::now();

            auto treeStart = Clock::now();
            bool treeInserted = tree.insert(movie);
            auto treeEnd = Clock::now();

            long long hashTime =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    hashEnd - hashStart
                ).count();

            long long treeTime =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    treeEnd - treeStart
                ).count();

            if (hashInserted && treeInserted) {
                movies.push_back(movie);

                std::cout << "Movie inserted successfully.\n";
            } else {
                std::cout
                    << "Insertion failed. The ID may already exist.\n";

                // Keep both structures consistent if only one insertion worked.
                if (hashInserted) {
                    hashTable.remove(id);
                }

                if (treeInserted) {
                    tree.remove(id);
                }
            }

            std::cout << "Hash Table insertion: "
                      << hashTime
                      << " ns\n";

            std::cout << "Red-Black Tree insertion: "
                      << treeTime
                      << " ns\n";
        }

        else if (choice == "4") {
            std::string id;

            std::cout << "Enter the IMDb ID to delete: ";
            std::getline(std::cin, id);

            auto hashStart = Clock::now();
            bool hashRemoved = hashTable.remove(id);
            auto hashEnd = Clock::now();

            auto treeStart = Clock::now();
            bool treeRemoved = tree.remove(id);
            auto treeEnd = Clock::now();

            long long hashTime =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    hashEnd - hashStart
                ).count();

            long long treeTime =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    treeEnd - treeStart
                ).count();

            if (hashRemoved && treeRemoved) {
                movies.erase(
                    std::remove_if(
                        movies.begin(),
                        movies.end(),
                        [&id](const Movie& movie) {
                            return movie.id == id;
                        }
                    ),
                    movies.end()
                );

                std::cout << "Movie deleted successfully.\n";
            } else {
                std::cout << "Movie was not found.\n";
            }

            std::cout << "Hash Table deletion: "
                      << hashTime
                      << " ns\n";

            std::cout << "Red-Black Tree deletion: "
                      << treeTime
                      << " ns\n";
        }

        else if (choice == "5") {
            std::cout
                << "\nInitial Dataset Performance\n"
                << "Movies loaded: "
                << movies.size()
                << '\n'
                << "Hash Table insertion: "
                << hashInsertionTime
                << " ms\n"
                << "Red-Black Tree insertion: "
                << treeInsertionTime
                << " ms\n"
                << "\nExpected complexities:\n"
                << "Hash Table search: Average O(1), worst O(n)\n"
                << "Hash Table insert: Average O(1), worst O(n)\n"
                << "Hash Table delete: Average O(1), worst O(n)\n"
                << "Red-Black Tree search: O(log n)\n"
                << "Red-Black Tree insert: O(log n)\n"
                << "Red-Black Tree delete: O(log n)\n"
                << "Title search: O(n)\n";
        }

        else if (choice == "6") {
            std::cout << "Exiting program.\n";
            break;
        }

        else {
            std::cout << "Invalid choice. Enter a number from 1 to 6.\n";
        }
    }

    return 0;
}