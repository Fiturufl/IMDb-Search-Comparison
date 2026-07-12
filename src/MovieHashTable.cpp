#include "MovieHashTable.h"

#include <functional>

MovieHashTable::Node::Node(
    const std::string& key,
    const Movie& movie
) : key(key), movie(movie), next(nullptr) {}

MovieHashTable::MovieHashTable(std::size_t capacity)
    : capacity(capacity),
      movieCount(0),
      table(capacity, nullptr) {}

MovieHashTable::~MovieHashTable() {
    clear();
}

// Isolate the identifying part of the ID
// (the numbers in tt0076759) and use as the hash.
std::size_t MovieHashTable::hashFunction(
    const std::string& key
) const {
    try {
        unsigned long long numericId =
            std::stoull(key.substr(2));

        return numericId % capacity;
    } catch (...) {
        return std::hash<std::string>{}(key) % capacity;
    }
}

// Insert a movie using its ID for the key
bool MovieHashTable::insert(const Movie& movie) {
    std::size_t index = hashFunction(movie.id);
    Node* ptr = table[index];
    Node* prev = nullptr;

    if (ptr == nullptr) {
        table[index] = new Node(movie.id, movie);
        movieCount++;
        return true;
    }
    // This is our base case, if the block was empty.

    while (ptr != nullptr) {
        if (ptr->key == movie.id) {
            return false;
        }

        prev = ptr;
        ptr = ptr->next;
    }

    prev->next = new Node(movie.id, movie);
    movieCount++;

    return true;
}

// Lookup: Returns either the movie pointer or a nullptr
Movie* MovieHashTable::search(const std::string& id) {
    std::size_t index = hashFunction(id);
    Node* ptr = table[index];

    while (ptr != nullptr) {
        if (ptr->key == id) {
            return &ptr->movie;
        }

        ptr = ptr->next;
    }

    return nullptr;
}

const Movie* MovieHashTable::search(
    const std::string& id
) const {
    std::size_t index = hashFunction(id);
    Node* ptr = table[index];

    while (ptr != nullptr) {
        if (ptr->key == id) {
            return &ptr->movie;
        }

        ptr = ptr->next;
    }

    return nullptr;
}

// Deletion: Returns true if successful, false if the item did not exist
bool MovieHashTable::remove(const std::string& id) {
    std::size_t index = hashFunction(id);
    Node* ptr = table[index];
    Node* prev = nullptr;

    // The code here is similar to that of the search,
    // with a prev ptr added in.
    while (ptr != nullptr) {
        if (ptr->key == id) {
            if (prev != nullptr) {
                prev->next = ptr->next;
            } else {
                table[index] = ptr->next;
            }

            delete ptr;
            movieCount--;

            return true;
        }

        prev = ptr;
        ptr = ptr->next;
    }

    return false;
}

std::size_t MovieHashTable::size() const {
    return movieCount;
}

void MovieHashTable::clear() {
    for (Node*& head : table) {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    movieCount = 0;
}