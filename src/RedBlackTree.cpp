#include "RedBlackTree.h"

#include <iostream>

// ============================================================
// Constructor and Destructor
// ============================================================

RedBlackTree::RedBlackTree() : root(nullptr) {}

RedBlackTree::~RedBlackTree() {
    destroyTree(root);
}


// ============================================================
// Public Operations
// ============================================================

bool RedBlackTree::insert(const Movie& movie) {
    Node* newNode = new Node(movie);

    // The first node becomes the black root.
    if (root == nullptr) {
        root = newNode;
        root->color = Color::BLACK;
        return true;
    }

    Node* current = root;
    Node* parent = nullptr;

    // Perform normal Binary Search Tree insertion.
    while (current != nullptr) {
        parent = current;

        // Movie IDs must be unique.
        if (movie.id == current->movie.id) {
            delete newNode;
            return false;
        }

        if (movie.id < current->movie.id) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    newNode->parent = parent;

    if (movie.id < parent->movie.id) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    // Restore Red-Black Tree properties.
    fixInsert(newNode);

    return true;
}

bool RedBlackTree::remove(const std::string& id) {
    Node* target = root;

    while (target != nullptr && target->movie.id != id) {
        if (id < target->movie.id) {
            target = target->left;
        } else {
            target = target->right;
        }
    }

    if (target == nullptr) {
        return false;
    }

    Node* replacement = target;
    Color originalColor = replacement->color;

    Node* child = nullptr;
    Node* childParent = nullptr;

    if (target->left == nullptr) {
        child = target->right;
        childParent = target->parent;
        transplant(target, target->right);
    } else if (target->right == nullptr) {
        child = target->left;
        childParent = target->parent;
        transplant(target, target->left);
    } else {
        replacement = minimum(target->right);
        originalColor = replacement->color;
        child = replacement->right;

        if (replacement->parent == target) {
            childParent = replacement;

            if (child != nullptr) {
                child->parent = replacement;
            }
        } else {
            childParent = replacement->parent;

            transplant(replacement, replacement->right);

            replacement->right = target->right;
            replacement->right->parent = replacement;
        }

        transplant(target, replacement);

        replacement->left = target->left;
        replacement->left->parent = replacement;
        replacement->color = target->color;
    }

    delete target;

    if (originalColor == Color::BLACK) {
        if (child != nullptr) {
            childParent = child->parent;
        }

        fixDelete(child, childParent);
    }

    return true;
}

Movie* RedBlackTree::search(const std::string& id) {
    Node* current = root;

    while (current != nullptr) {
        if (id == current->movie.id) {
            return &current->movie;
        }

        if (id < current->movie.id) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}

void RedBlackTree::printInOrder() const {
    printInOrder(root);
}


// ============================================================
// General Helper Functions
// ============================================================

void RedBlackTree::destroyTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);

    delete node;
}

void RedBlackTree::printInOrder(Node* node) const {
    if (node == nullptr) {
        return;
    }

    printInOrder(node->left);

    std::cout
        << node->movie.id << " | "
        << node->movie.title << " | "
        << (node->color == Color::RED ? "RED" : "BLACK")
        << '\n';

    printInOrder(node->right);
}

RedBlackTree::Node* RedBlackTree::minimum(Node* node) const {
    while (node != nullptr && node->left != nullptr) {
        node = node->left;
    }

    return node;
}

void RedBlackTree::transplant(Node* oldNode, Node* newNode) {
    if (oldNode->parent == nullptr) {
        root = newNode;
    } else if (oldNode == oldNode->parent->left) {
        oldNode->parent->left = newNode;
    } else {
        oldNode->parent->right = newNode;
    }

    if (newNode != nullptr) {
        newNode->parent = oldNode->parent;
    }
}

RedBlackTree::Color RedBlackTree::getColor(Node* node) const {
    // Null children are treated as black in a Red-Black Tree.
    if (node == nullptr) {
        return Color::BLACK;
    }

    return node->color;
}


// ============================================================
// Rotations
// ============================================================

void RedBlackTree::leftRotate(Node* node) {
    Node* rightChild = node->right;

    node->right = rightChild->left;

    if (rightChild->left != nullptr) {
        rightChild->left->parent = node;
    }

    rightChild->parent = node->parent;

    if (node->parent == nullptr) {
        root = rightChild;
    } else if (node == node->parent->left) {
        node->parent->left = rightChild;
    } else {
        node->parent->right = rightChild;
    }

    rightChild->left = node;
    node->parent = rightChild;
}

void RedBlackTree::rightRotate(Node* node) {
    Node* leftChild = node->left;

    node->left = leftChild->right;

    if (leftChild->right != nullptr) {
        leftChild->right->parent = node;
    }

    leftChild->parent = node->parent;

    if (node->parent == nullptr) {
        root = leftChild;
    } else if (node == node->parent->right) {
        node->parent->right = leftChild;
    } else {
        node->parent->left = leftChild;
    }

    leftChild->right = node;
    node->parent = leftChild;
}


// ============================================================
// Insertion Balancing
// ============================================================

void RedBlackTree::fixInsert(Node* node) {
    while (node != root &&
           node->parent->color == Color::RED) {

        Node* parent = node->parent;
        Node* grandparent = parent->parent;

        // Parent is the grandparent's left child.
        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;

            // Case 1: Parent and uncle are both red.
            if (getColor(uncle) == Color::RED) {
                parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                grandparent->color = Color::RED;
                node = grandparent;
            } else {
                // Case 2: Node forms a left-right triangle.
                if (node == parent->right) {
                    node = parent;
                    leftRotate(node);

                    parent = node->parent;
                    grandparent = parent->parent;
                }

                // Case 3: Node forms a left-left line.
                parent->color = Color::BLACK;
                grandparent->color = Color::RED;
                rightRotate(grandparent);
            }
        }

        // Parent is the grandparent's right child.
        else {
            Node* uncle = grandparent->left;

            // Mirror Case 1: Parent and uncle are both red.
            if (getColor(uncle) == Color::RED) {
                parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                grandparent->color = Color::RED;
                node = grandparent;
            } else {
                // Mirror Case 2: Node forms a right-left triangle.
                if (node == parent->left) {
                    node = parent;
                    rightRotate(node);

                    parent = node->parent;
                    grandparent = parent->parent;
                }

                // Mirror Case 3: Node forms a right-right line.
                parent->color = Color::BLACK;
                grandparent->color = Color::RED;
                leftRotate(grandparent);
            }
        }
    }

    root->color = Color::BLACK;
}

void RedBlackTree::fixDelete(Node* node, Node* parent) {
    while (node != root && getColor(node) == Color::BLACK) {
        if (parent == nullptr) {
            break;
        }

        if (node == parent->left) {
            Node* sibling = parent->right;

            // Case 1: Sibling is red.
            if (getColor(sibling) == Color::RED) {
                sibling->color = Color::BLACK;
                parent->color = Color::RED;
                leftRotate(parent);
                sibling = parent->right;
            }

            Node* siblingLeft =
                sibling == nullptr ? nullptr : sibling->left;
            Node* siblingRight =
                sibling == nullptr ? nullptr : sibling->right;

            // Case 2: Sibling and both children are black.
            if (getColor(siblingLeft) == Color::BLACK &&
                getColor(siblingRight) == Color::BLACK) {

                if (sibling != nullptr) {
                    sibling->color = Color::RED;
                }

                node = parent;
                parent = node->parent;
            } else {
                // Case 3: Sibling's far child is black.
                if (getColor(siblingRight) == Color::BLACK) {
                    if (siblingLeft != nullptr) {
                        siblingLeft->color = Color::BLACK;
                    }

                    if (sibling != nullptr) {
                        sibling->color = Color::RED;
                        rightRotate(sibling);
                    }

                    sibling = parent->right;
                }

                // Case 4: Sibling's far child is red.
                if (sibling != nullptr) {
                    sibling->color = parent->color;
                }

                parent->color = Color::BLACK;

                if (sibling != nullptr &&
                    sibling->right != nullptr) {
                    sibling->right->color = Color::BLACK;
                }

                leftRotate(parent);
                node = root;
                parent = nullptr;
            }
        } else {
            Node* sibling = parent->left;

            // Mirror Case 1: Sibling is red.
            if (getColor(sibling) == Color::RED) {
                sibling->color = Color::BLACK;
                parent->color = Color::RED;
                rightRotate(parent);
                sibling = parent->left;
            }

            Node* siblingLeft =
                sibling == nullptr ? nullptr : sibling->left;
            Node* siblingRight =
                sibling == nullptr ? nullptr : sibling->right;

            // Mirror Case 2.
            if (getColor(siblingLeft) == Color::BLACK &&
                getColor(siblingRight) == Color::BLACK) {

                if (sibling != nullptr) {
                    sibling->color = Color::RED;
                }

                node = parent;
                parent = node->parent;
            } else {
                // Mirror Case 3.
                if (getColor(siblingLeft) == Color::BLACK) {
                    if (siblingRight != nullptr) {
                        siblingRight->color = Color::BLACK;
                    }

                    if (sibling != nullptr) {
                        sibling->color = Color::RED;
                        leftRotate(sibling);
                    }

                    sibling = parent->left;
                }

                // Mirror Case 4.
                if (sibling != nullptr) {
                    sibling->color = parent->color;
                }

                parent->color = Color::BLACK;

                if (sibling != nullptr &&
                    sibling->left != nullptr) {
                    sibling->left->color = Color::BLACK;
                }

                rightRotate(parent);
                node = root;
                parent = nullptr;
            }
        }
    }

    if (node != nullptr) {
        node->color = Color::BLACK;
    }
}