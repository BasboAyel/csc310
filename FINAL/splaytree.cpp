#include "splaytree.h"
#include <iostream>

using namespace std;

SplayTree::SplayTree() : root(nullptr) {}

SplayTree::~SplayTree() {
    freeTree(root);
}

SplayNode* SplayTree::rightRotate(SplayNode* x) {
    SplayNode* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

SplayNode* SplayTree::leftRotate(SplayNode* x) {
    SplayNode* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

SplayNode* SplayTree::splay(SplayNode* root, int x, int y) {
    if (!root || (root->x == x && root->y == y))
        return root;

    if (x < root->x || (x == root->x && y < root->y)) {
        if (!root->left) return root;
        if (x < root->left->x || (x == root->left->x && y < root->left->y)) {
            root->left->left = splay(root->left->left, x, y);
            root = rightRotate(root);
        } else if (x > root->left->x || (x == root->left->x && y > root->left->y)) {
            root->left->right = splay(root->left->right, x, y);
            if (root->left->right)
                root->left = leftRotate(root->left);
        }
        return (root->left == nullptr) ? root : rightRotate(root);
    } else {
        if (!root->right) return root;
        if (x > root->right->x || (x == root->right->x && y > root->right->y)) {
            root->right->right = splay(root->right->right, x, y);
            root = leftRotate(root);
        } else if (x < root->right->x || (x == root->right->x && y < root->right->y)) {
            root->right->left = splay(root->right->left, x, y);
            if (root->right->left)
                root->right = rightRotate(root->right);
        }
        return (root->right == nullptr) ? root : leftRotate(root);
    }
}

SplayNode* SplayTree::insert(SplayNode* root, int x, int y) {
    if (!root)
        return new SplayNode(x, y);

    root = splay(root, x, y);

    if (root->x == x && root->y == y)
        return root;

    SplayNode* newNode = new SplayNode(x, y);

    if (x < root->x || (x == root->x && y < root->y)) {
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

void SplayTree::insert(int x, int y) {
    root = insert(root, x, y);
}

bool SplayTree::find(int x, int y) {
    root = splay(root, x, y);
    return root && root->x == x && root->y == y;
}

void SplayTree::freeTree(SplayNode* node) {
    if (node) {
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }
}
