#ifndef SPLAYTREE_H
#define SPLAYTREE_H

struct SplayNode {
    int x, y;
    SplayNode* left;
    SplayNode* right;

    SplayNode(int x, int y) : x(x), y(y), left(nullptr), right(nullptr) {}
};

class SplayTree {
private:
    SplayNode* root;
    SplayNode* rightRotate(SplayNode* x);
    SplayNode* leftRotate(SplayNode* x);
    SplayNode* splay(SplayNode* root, int x, int y);
    SplayNode* insert(SplayNode* root, int x, int y);
    void freeTree(SplayNode* node);
public:
    SplayTree();
    ~SplayTree();
    void insert(int x, int y);
    bool find(int x, int y);
};

#endif
