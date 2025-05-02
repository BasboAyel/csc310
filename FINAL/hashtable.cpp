#include "hashtable.h"

HashTable::HashTable() {
    table.resize(TABLE_SIZE, nullptr);
}

HashTable::~HashTable() {
    for (auto node : table) {
        while (node) {
            HashNode* temp = node;
            node = node->next;
            delete temp;
        }
    }
}

int HashTable::hashFunction(int x, int y) {
    return (x * 31 + y) % TABLE_SIZE;
}

void HashTable::insert(int x, int y) {
    int idx = hashFunction(x, y);
    HashNode* newNode = new HashNode(x, y);
    newNode->next = table[idx];
    table[idx] = newNode;
}

bool HashTable::search(int x, int y) {
    int idx = hashFunction(x, y);
    HashNode* curr = table[idx];
    while (curr) {
        if (curr->x == x && curr->y == y)
            return true;
        curr = curr->next;
    }
    return false;
}

void HashTable::remove(int x, int y) {
    int idx = hashFunction(x, y);
    HashNode* curr = table[idx];
    HashNode* prev = nullptr;
    while (curr) {
        if (curr->x == x && curr->y == y) {
            if (prev) prev->next = curr->next;
            else table[idx] = curr->next;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
