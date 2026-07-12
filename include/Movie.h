#ifndef MOVIE_H
#define MOVIE_H

#include <string>

struct Movie {
    std::string id;
    std::string title;
    int year;
    float rating;
    Movie() = default;

    Movie(const std::string& id,
          const std::string& title,
          int year,
          float rating)
        : id(id), title(title), year(year), rating(rating) {}

};

#endif