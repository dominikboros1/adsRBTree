#include <iostream>
#include <fstream>
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
            } else {
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

    void visualizeTree(Node* root, int space, int height, std::ostream& out) const {
        if (!root)
            return;

        space += height;

        visualizeTree(root->right, space, height, out);

        for (int i = height; i < space; ++i)
            out << " ";
        out << root->data << (root->color == RED ? "R" : "B") << "\n";

        visualizeTree(root->left, space, height, out);
    }

    Node* deleteBST(Node* root, int data) {
        if (!root)
            return root;

        if (data < root->data) {
            root->left = deleteBST(root->left, data);
        } else if (data > root->data) {
            root->right = deleteBST(root->right, data);
        } else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;

                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }

                delete temp;
            } else {
                Node* temp = minValueNode(root->right);
                root->data = temp->data;
                root->right = deleteBST(root->right, temp->data);
            }
        }

        return root;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left)
            current = current->left;
        return current;
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

    void deleteNode(int data) {
        root = deleteBST(root, data);
    }

    void display() const {
        if (root)
            visualizeTree(root, 0, 10, std::cout);
        else
            std::cout << "Tree is empty.\n";
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            if (root)
                visualizeTree(root, 0, 10, file);
            else
                file << "Tree is empty.\n";
            file.close();
            std::cout << "Tree saved to " << filename << "\n";
        } else {
            std::cout << "Error: Unable to open file.\n";
        }
    }
};

int main() {
    RBTree tree;
    int choice;

    do {
        std::cout << "\nMenu:\n"
                  << "1. Insert Node\n"
                  << "2. Delete Node\n"
                  << "3. Display Tree\n"
                  << "4. Save Tree to File\n"
                  << "5. Exit\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int value;
                std::cout << "Enter value to insert: ";
                std::cin >> value;
                tree.insert(value);
                break;
            }
            case 2: {
                int value;
                std::cout << "Enter value to delete: ";
                std::cin >> value;
                tree.deleteNode(value);
                break;
            }
            case 3:
                std::cout << "\nTree Visualization:\n";
                tree.display();
                break;
            case 4: {
                std::string filename;
                std::cout << "Enter filename to save: ";
                std::cin >> filename;
                tree.saveToFile(filename);
                break;
            }
            case 5:
                std::cout << "Exiting Program\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
