//
// Created by Lori on 7/11/26.
//

#include "MovieHashTable.h"

#include <string>
#include <vector>



/*struct Movie {
    std::string id;
    std::string title;
    int year;
    float rating;
};*/ // Ambiguous; Francisco's code already defined it.

struct MovieHashNode {
    std::string key;
    Movie* moviePtr;
    MovieHashNode* next;

    MovieHashNode(std::string  k, Movie* m) : key(k), moviePtr(m), next(nullptr) {}
};

class MovieHashTable {
    int capacity = 800011;
    // We use a prime for our hash table's size for safety.

    MovieHashNode* table[800011] = {nullptr};
    // We don't even need to use a load factor either in this Hash Table, because
    // we know how large our data is going to be before we begin.

    // Isolate the identifying part of the ID (the numbers in tt0076759) and use as the hash.
    int hashFunction(std::string& key) {
        int numericId = std::stoi(key.substr(2));
        return numericId % capacity;
    }

public:
    // Resizing function is currently unnecessary, because we know the size of the data.

    // Insert a movie using its ID for the key and its pointer for the value
    void insertMovie(std::string& key, Movie* movie) {
        int index = hashFunction(key);
        MovieHashNode* newNode = new MovieHashNode(key, movie);

        MovieHashNode* ptr = table[index];
        MovieHashNode* prev = nullptr;

        if (ptr == nullptr) {
            table[index] = newNode;
            return;
        }
        // This is our base case, if the block waas empty

        while (ptr != nullptr) {
            if (ptr->key == key) {
                newNode->next = ptr->next;
                if (prev != nullptr) prev->next = newNode;
                else table[index] = newNode;
            }
            ptr = ptr->next;
            prev = ptr;
        }
    }

    // Lookup: Returns either the movie pointer or a nullptr
    Movie* searchMovie(std::string& key) {
        int index = hashFunction(key);
        MovieHashNode* ptr = table[index];

        while (ptr != nullptr) {
            if (ptr->key == key) {
                return ptr->moviePtr;
            }
            ptr = ptr->next;
        }

        return nullptr;
    }

    // Deletion: Returns true if successful, false if the item did not exist
    bool deleteMovie(std::string& key) {
        int index = hashFunction(key);
        MovieHashNode* ptr = table[index];
        MovieHashNode* prev = nullptr;
        // the code here is similar to that of the search, with a prev ptr added in

        while (ptr != nullptr) {
            if (ptr->key == key) {
                if (prev != nullptr) prev->next = ptr->next;
                else table[index] = ptr->next;
                delete ptr;
                // Just freeing up my MovieHashNode. NOT the Movie itself, which the other tree needs.
                return true;
            }
            prev = ptr;
            ptr = ptr->next;
        }

        return false;

    }
};
