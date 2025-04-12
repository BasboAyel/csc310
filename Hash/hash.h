#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include "AVL/avl.h"

using namespace std;

enum CollisionHandling {
    CHAINING_VECTOR,
    CHAINING_LIST,
    CHAINING_BST,
    LINEAR_PROBING,
    QUADRATIC_PROBING,
    DOUBLE_HASHING
};

class HashTable {
    private:
        int tableSize;
        int elementCount;
        CollisionHandling method;

        // Storage containers for the different variants
        vector<vector<pair<string, int>>> tableVector;
        vector<list<pair<string, int>>> tableList;
        vector<AVLTree> tableBST; // (uses AVLTree)
        vector<pair<string, int>> tableProbing;

        // Hash Functions and Collision Handling
        int hash1(const string& key) const;
        int hash2(const string& key) const;  
        int probe(int index, int i) const;   // COMPLETE THIS
        void rehash(); // COMPLETE THIS

        // Helper functions
        int findEmptySlot(const string& key); // COMPLETE THIS
        void resizeIfNeeded(); // COMPLETE THIS

    public:
        HashTable(int size, CollisionHandling variant);
        ~HashTable();

        void insert(const string& key, int value); // COMPLETE THIS
        bool search(const string& key, int& value); // COMPLETE THIS
        bool remove(const string& key); // COMPLETE THIS

        // Benchmark and Display
        void benchmarkHashTable(HashTable& table, const vector<pair<string, int>>& data, int numSearch, int numDelete);
        void displayStats();
};

vector<pair<string, int>> readDataFromFile(const string& filename);
void benchmarkStdUnorderedMap(const vector<pair<string, int>>& data, int numSearch, int numDelete);

#endif


#include "hash_table.h"

int HashTable::hash1(const string& key) const {
    int hashValue = 0;
    for (char c : key) {
        hashValue = (hashValue * 31 + c) % tableSize;
    }
    return hashValue;
}

int HashTable::hash2(const string& key) const {
    int hashValue = 0;
    for (char c : key) {
        hashValue = (hashValue * 17 + c) % tableSize;
    }
    return 1 + (hashValue % (tableSize - 1));  // Ensure it's non-zero for double hashing
}

int HashTable::probe(int index, int i) const {
    switch (method) {
        case LINEAR_PROBING:
            return (index + i) % tableSize;
        case QUADRATIC_PROBING:
            return (index + i * i) % tableSize;
        case DOUBLE_HASHING:
            return (index + i * hash2("")) % tableSize;  // Double hash
        default:
            return index;
    }
}

void HashTable::rehash() {
    int oldSize = tableSize;
    tableSize *= 2;
    vector<pair<string, int>> oldData;

    switch (method) {
        case CHAINING_VECTOR:
            for (auto& bucket : tableVector) {
                for (auto& p : bucket) {
                    oldData.push_back(p);
                }
            }
            break;
        case CHAINING_LIST:
            for (auto& bucket : tableList) {
                for (auto& p : bucket) {
                    oldData.push_back(p);
                }
            }
            break;
        case CHAINING_BST:
            for (auto& tree : tableBST) {
                vector<pair<string, int>> bstData = tree.getAllElements();
                oldData.insert(oldData.end(), bstData.begin(), bstData.end());
            }
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            for (auto& p : tableProbing) {
                if (!p.first.empty()) {
                    oldData.push_back(p);
                }
            }
            break;
    }

    switch (method) {
        case CHAINING_VECTOR:
            tableVector.clear();
            tableVector.resize(tableSize);
            break;
        case CHAINING_LIST:
            tableList.clear();
            tableList.resize(tableSize);
            break;
        case CHAINING_BST:
            tableBST.clear();
            tableBST.resize(tableSize);
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            tableProbing.clear();
            tableProbing.resize(tableSize, make_pair("", -1));
            break;
    }

    elementCount = 0;
    for (const auto& p : oldData) {
        insert(p.first, p.second);
    }
}

int HashTable::findEmptySlot(const string& key) {
    int index = hash1(key) % tableSize;
    int i = 0;
    while (!tableProbing[index].first.empty() && tableProbing[index].first != key) {
        i++;
        index = probe(index, i);
    }
    return index;
}

void HashTable::resizeIfNeeded() {
    if (elementCount > tableSize * 0.7) {
        rehash();
    }
}

void HashTable::insert(const string& key, int value) {
    resizeIfNeeded();

    int index = hash1(key) % tableSize;
    switch (method) {
        case CHAINING_VECTOR:
            tableVector[index].push_back(make_pair(key, value));
            break;
        case CHAINING_LIST:
            tableList[index].push_back(make_pair(key, value));
            break;
        case CHAINING_BST:
            tableBST[index].insert(key, value);
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            index = findEmptySlot(key);
            tableProbing[index] = make_pair(key, value);
            break;
    }
    elementCount++;
}

bool HashTable::search(const string& key, int& value) {
    int index = hash1(key) % tableSize;
    switch (method) {
        case CHAINING_VECTOR:
            for (const auto& p : tableVector[index]) {
                if (p.first == key) {
                    value = p.second;
                    return true;
                }
            }
            break;
        case CHAINING_LIST:
            for (const auto& p : tableList[index]) {
                if (p.first == key) {
                    value = p.second;
                    return true;
                }
            }
            break;
        case CHAINING_BST:
            return tableBST[index].search(key, value);
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            while (!tableProbing[index].first.empty()) {
                if (tableProbing[index].first == key) {
                    value = tableProbing[index].second;
                    return true;
                }
                index = probe(index, 1);
            }
            break;
    }
    return false;
}

bool HashTable::remove(const string& key) {
    int index = hash1(key) % tableSize;
    switch (method) {
        case CHAINING_VECTOR:
            for (auto it = tableVector[index].begin(); it != tableVector[index].end(); ++it) {
                if (it->first == key) {
                    tableVector[index].erase(it);
                    elementCount--;
                    return true;
                }
            }
            break;
        case CHAINING_LIST:
            for (auto it = tableList[index].begin(); it != tableList[index].end(); ++it) {
                if (it->first == key) {
                    tableList[index].erase(it);
                    elementCount--;
                    return true;
                }
            }
            break;
        case CHAINING_BST:
            if (tableBST[index].remove(key)) {
                elementCount--;
                return true;
            }
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            while (!tableProbing[index].first.empty()) {
                if (tableProbing[index].first == key) {
                    tableProbing[index] = make_pair("", -1);
                    elementCount--;
                    return true;
                }
                index = probe(index, 1);
            }
            break;
    }
    return false;
}
