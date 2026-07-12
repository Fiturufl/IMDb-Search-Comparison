//
// Created by Lori on 7/11/26.
//

#pragma once
#include <string>

struct Movie {
    std::string id;
    std::string title;
    int year;
    float rating;
};

struct MovieHashNode {
    std::string key;
    Movie* moviePtr;
    MovieHashNode* next;

    MovieHashNode(std::string  k, Movie* m) : key(k), moviePtr(m), next(nullptr) {}
};

class MovieHashTable {
    int capacity;
    MovieHashNode* table[800011];
public:
    MovieHashTable();
    ~MovieHashTable();
    void insertMovie(std::string& key, Movie* movie);
    Movie* searchMovie(std::string& key);
};
