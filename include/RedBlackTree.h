#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include "Movie.h"

#include <string>

class RedBlackTree {
private:
    enum class Color {
        RED,
        BLACK
    };

    struct Node {
        Movie movie;
        Color color;
        Node* parent;
        Node* left;
        Node* right;

        explicit Node(const Movie& movie)
            : movie(movie),
              color(Color::RED),
              parent(nullptr),
              left(nullptr),
              right(nullptr) {}
    };

    Node* root;

    // General helpers
    void destroyTree(Node* node);
    void printInOrder(Node* node) const;

    Node* minimum(Node* node) const;
    void transplant(Node* oldNode, Node* newNode);
    Color getColor(Node* node) const;

    // Rotations
    void leftRotate(Node* node);
    void rightRotate(Node* node);

    // Balancing
    void fixInsert(Node* node);
    void fixDelete(Node* node, Node* parent);

public:
    RedBlackTree();
    ~RedBlackTree();

    bool insert(const Movie& movie);
    bool remove(const std::string& id);
    Movie* search(const std::string& id);

    void printInOrder() const;
};

#endif