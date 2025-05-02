#include "avltree.h"
#include <iostream>

using namespace std;

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    freeTree(root);
}

int AVLTree::height(AVLNode* N) {
    return N ? N->height : 0;
}

int AVLTree::getBalance(AVLNode* N) {
    return N ? height(N->left) - height(N->right) : 0;
}

AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* AVLTree::insert(AVLNode* node, int timestamp, int score) {
    if (!node)
        return new AVLNode(timestamp, score);

    if (timestamp < node->timestamp)
        node->left = insert(node->left, timestamp, score);
    else if (timestamp > node->timestamp)
        node->right = insert(node->right, timestamp, score);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && timestamp < node->left->timestamp)
        return rightRotate(node);

    if (balance < -1 && timestamp > node->right->timestamp)
        return leftRotate(node);

    if (balance > 1 && timestamp > node->left->timestamp) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && timestamp < node->right->timestamp) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::insert(int timestamp, int score) {
    root = insert(root, timestamp, score);
}

void AVLTree::inOrder(AVLNode* root) {
    if (root) {
        inOrder(root->left);
        cout << "Fruit eaten at time: " << root->timestamp << ", Score: " << root->score << endl;
        inOrder(root->right);
    }
}

void AVLTree::display() {
    inOrder(root);
}

void AVLTree::freeTree(AVLNode* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        delete root;
    }
}
