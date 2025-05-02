#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>

struct HashNode {
    int x, y;
    HashNode* next;
    HashNode(int x, int y) : x(x), y(y), next(nullptr) {}
};

class HashTable {
private:
    static const int TABLE_SIZE = 100;
    std::vector<HashNode*> table;
    int hashFunction(int x, int y);
public:
    HashTable();
    ~HashTable();
    void insert(int x, int y);
    bool search(int x, int y);
    void remove(int x, int y);
};

#endif
