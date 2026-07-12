#ifndef MOVIE_HASH_TABLE_H
#define MOVIE_HASH_TABLE_H

#include "Movie.h"

#include <cstddef>
#include <string>
#include <vector>

class MovieHashTable {
private:
    struct Node {
        std::string key;
        Movie movie;
        Node* next;

        Node(const std::string& key, const Movie& movie);
    };

    static constexpr std::size_t DEFAULT_CAPACITY = 800011;

    std::size_t capacity;
    std::size_t movieCount;
    std::vector<Node*> table;

    std::size_t hashFunction(const std::string& key) const;
    void clear();

public:
    explicit MovieHashTable(
        std::size_t capacity = DEFAULT_CAPACITY
    );

    ~MovieHashTable();

    MovieHashTable(const MovieHashTable&) = delete;
    MovieHashTable& operator=(const MovieHashTable&) = delete;

    bool insert(const Movie& movie);
    Movie* search(const std::string& id);
    const Movie* search(const std::string& id) const;
    bool remove(const std::string& id);

    std::size_t size() const;
};

#endif