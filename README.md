# IMDb Search Comparison

This project compares the performance of a custom Hash Table and a custom Red-Black Tree using the official IMDb movie title dataset.

## Team

**Team Name:** San Frax

**Team Members:**

- Francisco Iturriaga
- Murat Atar
- Tristan Cole

## Project Overview

The goal of this project is to compare two data structures that store and search a large movie dataset.

The program loads IMDb movie records into:

- A Hash Table
- A Red-Black Tree

Both data structures are implemented from scratch and use the IMDb movie ID as the key.

The program also includes a simple menu-driven interface.

## Features

The program can:

- Load the IMDb title dataset
- Search movies by title
- Search movies by IMDb ID
- Insert a movie record
- Delete a movie record
- Compare insertion performance
- Compare search performance
- Compare deletion performance
- Display asymptotic time complexities

## Data Structures

### Hash Table

The Hash Table uses separate chaining to handle collisions.

Expected time complexities:

- Search: Average `O(1)`, worst case `O(n)`
- Insert: Average `O(1)`, worst case `O(n)`
- Delete: Average `O(1)`, worst case `O(n)`

### Red-Black Tree

The Red-Black Tree is a self-balancing binary search tree. It uses rotations and recoloring to maintain balance.

Expected time complexities:

- Search: `O(log n)`
- Insert: `O(log n)`
- Delete: `O(log n)`

## Dataset

This project uses the official IMDb title dataset.

Download the dataset here:

https://datasets.imdbws.com/title.basics.tsv.gz

After downloading:

1. Extract `title.basics.tsv.gz`.
2. Create a folder named `data` in the project folder.
3. Place the extracted file inside the `data` folder.

The final file path should be:

```text
data/title.basics.tsv
```

The full dataset is not included in the repository because it is too large for normal GitHub storage.

## Project Structure

```text
IMDb-Search-Comparison/
├── data/
│   └── title.basics.tsv
├── include/
│   ├── DatasetLoader.h
│   ├── Movie.h
│   ├── MovieHashTable.h
│   └── RedBlackTree.h
├── src/
│   ├── DatasetLoader.cpp
│   ├── MovieHashTable.cpp
│   ├── RedBlackTree.cpp
│   └── main.cpp
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## Requirements

- C++17
- GCC or another compatible C++ compiler
- MSYS2 MinGW64 is recommended for Windows

## Compile and Run

Open the MSYS2 MinGW64 terminal.

Move into the project folder:

```bash
Open a terminal in the project folder.
cd /path/to/IMDb-Search-Comparison
```

Compile the program:

```bash
g++ -std=c++17 -Iinclude src/main.cpp src/DatasetLoader.cpp src/RedBlackTree.cpp src/MovieHashTable.cpp -o app.exe
```

Run the program:

```bash
./app.exe
```

## Menu Options

The program displays the following menu:

```text
====================================
       IMDb Movie Search Menu
====================================
1. Search movie by title
2. Search movie by ID
3. Insert movie
4. Delete movie
5. Show performance results
6. Exit
```

## Searching by Title

The title search:

- Is case-insensitive
- Supports partial title matches
- Displays up to the first 20 matching movies

Because the Hash Table and Red-Black Tree are keyed by IMDb ID, title searching scans the loaded movie vector.

The time complexity of title searching is:

```text
O(n)
```

## Searching by IMDb ID

The user can enter an IMDb ID such as:

```text
tt0111161
```

The program searches for the movie in both the Hash Table and the Red-Black Tree.

The program displays:

- Movie ID
- Movie title
- Release year
- Hash Table search time
- Red-Black Tree search time

## Inserting a Movie

The user can enter:

- IMDb ID
- Movie title
- Release year

The new movie is inserted into both data structures.

The IMDb ID must be unique.

## Deleting a Movie

The user can enter an IMDb ID to delete.

The movie is removed from:

- The Hash Table
- The Red-Black Tree
- The movie vector used for title searching

The program also displays deletion times for both data structures.

## Example Performance Results

Performance results depend on the computer running the program.

Example results using 638,563 movie records:

```text
Insertion Results
Hash Table: 178 ms
Red-Black Tree: 761 ms

Search Results
Hash Table: 1900 ns
Red-Black Tree: 2800 ns

Deletion Results
Hash Table: 1500 ns
Red-Black Tree: 2600 ns
```

In this example, the Hash Table performed faster than the Red-Black Tree for insertion, search, and deletion.

## Responsibilities

### Murat Atar

- Menu integration
- User interaction
- Connecting the Hash Table and Red-Black Tree functions
- Displaying search and performance results

### Tristan Cole

- Hash Table implementation
- Timing reports using `std::chrono`
- Asymptotic analysis

### Francisco Iturriaga

- Red-Black Tree implementation
- Timing reports
- Asymptotic analysis

## GitHub Setup

The dataset and generated executable should not be committed to GitHub.

Recommended `.gitignore` contents:

```gitignore
build/
app.exe
data/*
!data/README.md
```

Create a file named:

```text
data/README.md
```

Place this inside it:

```md
# IMDb Dataset

Download the IMDb title dataset:

https://datasets.imdbws.com/title.basics.tsv.gz

Extract the file and place `title.basics.tsv` inside this folder.
```

## References

- IMDb Datasets: https://datasets.imdbws.com/
- IMDb Non-Commercial Datasets: https://developer.imdb.com/non-commercial-datasets/
- C++ Documentation: https://cplusplus.com/
