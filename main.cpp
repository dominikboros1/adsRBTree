#include <iostream>
#include <queue>
#include <iomanip>

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int data) : data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
private:
    Node* root;

    void rotateLeft(Node*& root, Node*& node) {
        Node* rightChild = node->right;
        node->right = rightChild->left;

        if (node->right)
            node->right->parent = node;

        rightChild->parent = node->parent;

        if (!node->parent)
            root = rightChild;
        else if (node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;

        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(Node*& root, Node*& node) {
        Node* leftChild = node->left;
        node->left = leftChild->right;

        if (node->left)
            node->left->parent = node;

        leftChild->parent = node->parent;

        if (!node->parent)
            root = leftChild;
        else if (node == node->parent->left)
            node->parent->left = leftChild;
        else
            node->parent->right = leftChild;

        leftChild->right = node;
        node->parent = leftChild;
    }

    void fixViolation(Node*& root, Node*& node) {
        Node* parent = nullptr;
        Node* grandparent = nullptr;

        while (node != root && node->color != BLACK && node->parent->color == RED) {
            parent = node->parent;
            grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                if (uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {

                    if (node == parent->right) {
                        rotateLeft(root, parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateRight(root, grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }

            else {
                Node* uncle = grandparent->left;

                if (uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {

                    if (node == parent->left) {
                        rotateRight(root, parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateLeft(root, grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = BLACK;
    }

    void visualizeTree(Node* root, int space = 0, int height = 10) const {
        if (!root)
            return;

        space += height;

        visualizeTree(root->right, space);

        std::cout << std::endl;
        for (int i = height; i < space; ++i)
            std::cout << " ";
        std::cout << root->data << (root->color == RED ? "R" : "B") << "\n";

        visualizeTree(root->left, space);
    }

public:
    RBTree() : root(nullptr) {}

    void insert(const int& data) {
        Node* node = new Node(data);
        root = insertBST(root, node);
        fixViolation(root, node);
    }

    Node* insertBST(Node* root, Node* node) {
        if (!root)
            return node;

        if (node->data < root->data) {
            root->left = insertBST(root->left, node);
            root->left->parent = root;
        } else if (node->data > root->data) {
            root->right = insertBST(root->right, node);
            root->right->parent = root;
        }

        return root;
    }

    void display() const {
        if (root)
            visualizeTree(root);
        else
            std::cout << "Tree is empty.\n";
    }
};

int main() {
    RBTree tree;

    int n;
    std::cout << "Enter the number of nodes you want to add: ";
    std::cin >> n;

    std::cout << "Enter the node values:\n";
    for (int i = 0; i < n; ++i) {
        int value;
        std::cout << "Node " << (i + 1) << ": ";
        std::cin >> value;
        tree.insert(value);
    }

    std::cout << "\nRed-Black Tree Visualization:\n";
    tree.display();

    return 0;
}
