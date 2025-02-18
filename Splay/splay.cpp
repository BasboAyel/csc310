#include "splay.h"
#include "customErrorClass.h"
#include <iostream>
 
using namespace std;
 
SplayTree::SplayTree() {
    root = nullptr;
}
 
// aka Zig
SplayTree::Node* SplayTree::rotateRight(Node* x) {
    if (!x || !x->left) throw MyException("Rotation Error: Cannot perform right rotation.");
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}
 
// aka Zag
SplayTree::Node* SplayTree::rotateLeft(Node* x) {
    if (!x || !x->right) throw MyException("Rotation Error: Cannot perform left rotation.");
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}
 
// Splaying
SplayTree::Node* SplayTree::splay(Node* root, int key) {
    if (!root)
        return nullptr;
   
    if (root->key == key) return root;
 
    try {
        if (key < root->key) {
            if (!root->left) return root;
 
            if (key < root->left->key) {
                root->left->left = splay(root->left->left, key);
                root = rotateRight(root);
            } else if (key > root->left->key) {
                root->left->right = splay(root->left->right, key);
                if (root->left->right)
                    root->left = rotateLeft(root->left);
            }
            return root->left ? rotateRight(root) : root;
        } else {
            if (!root->right) return root;
 
            if (key > root->right->key) {
                root->right->right = splay(root->right->right, key);
                root = rotateLeft(root);
            } else if (key < root->right->key) {
                root->right->left = splay(root->right->left, key);
                if (root->right->left)
                    root->right = rotateRight(root->right);
            }
            return root->right ? rotateLeft(root) : root;
        }
    } catch (MyException &e) {
        cerr << "Exception: " << e.what() << endl;
        return root;
    }
}
 
// Insert Node
SplayTree::Node* SplayTree::insertNode(Node* root, int key) {
    if (!root) return new Node(key);
 
    root = splay(root, key);
    if (root->key == key) {
        throw MyException("Insertion Error: Duplicate key insertion attempted.");
    }
 
    Node* newNode = new Node(key);
    if (key < root->key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = nullptr;
    } else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = nullptr;
    }
    return newNode;
}
 
// Delete Node
SplayTree::Node* SplayTree::deleteNode(Node* root, int key) {
    if (!root) throw MyException("Deletion Error: Attempted to delete from an empty tree.");
 
    root = splay(root, key);
    if (root->key != key) {
        throw MyException("Deletion Error: Key not found.");
    }
 
    Node* temp;
    if (!root->left) {
        temp = root->right;
    } else {
        temp = splay(root->left, key);
        temp->right = root->right;
    }
    delete root;
    return temp;
}
 
// Insert Wrapper
void SplayTree::insert(int key) {
    try {
        root = insertNode(root, key);
    } catch (MyException &e) {
        cerr << "Exception: " << e.what() << endl;
    }
}
 
// Remove Wrapper
void SplayTree::remove(int key) {
    try {
        root = deleteNode(root, key);
    } catch (MyException &e) {
        cerr << "Exception: " << e.what() << endl;
    }
}
 
// Search Wrapper
bool SplayTree::search(int key) {
    try {
        root = splay(root, key);
        return (root && root->key == key);
    } catch (MyException &e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
}
 
// Print Tree
void SplayTree::printTree(Node* root, int space) {
    const int COUNT = 10;
    if (!root) return;
 
    space += COUNT;
 
    printTree(root->right, space);
   
    for (int i = COUNT; i < space; i++) {
        cout << " ";
    }
    cout << root->key << endl;
 
    printTree(root->left, space);
}
 
// Display Wrapper
void SplayTree::display() {
    try {
        if (!root) throw MyException("Display Error: Tree is empty.");
        printTree(root, 0);
        cout << endl;
    } catch (MyException &e) {
        cerr << "Exception: " << e.what() << endl;
    }
}
 