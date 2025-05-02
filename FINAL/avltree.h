#ifndef AVLTREE_H
#define AVLTREE_H

struct AVLNode {
    int timestamp;
    int score;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int t, int s) : timestamp(t), score(s), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;
    int height(AVLNode* N);
    int getBalance(AVLNode* N);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode* node, int timestamp, int score);
    void inOrder(AVLNode* root);
    void freeTree(AVLNode* root);
public:
    AVLTree();
    ~AVLTree();
    void insert(int timestamp, int score);
    void display();
};

#endif
