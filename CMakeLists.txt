cmake_minimum_required(VERSION 3.20)

project(IMDbSearchComparison)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(IMDbSearchComparison
    src/main.cpp
    src/RedBlackTree.cpp
    src/DatasetLoader.cpp
        include/MovieHashTable.h
)
target_include_directories(IMDbSearchComparison PRIVATE include)
