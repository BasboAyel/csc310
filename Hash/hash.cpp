#include "hash.h"

// Constructor - initializing table based on chosen hash table variant
HashTable::HashTable(int size, CollisionHandling variant) 
{
    tableSize = size; 
    elementCount = 0; 
    method = variant;
    switch (method) {
        case CHAINING_VECTOR:
            tableVector.resize(size);
            break;
        case CHAINING_LIST:
            tableList.resize(size);
            break;
        case CHAINING_BST:
            tableBST.resize(size);
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            tableProbing.resize(size);
            break;
    }
}

HashTable::~HashTable() {}

int HashTable::hash1(const string& key) const {
    unsigned long hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c);
    }
    return hash % tableSize;
}

int HashTable::hash2(const string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return 1 + (hash % (tableSize - 1));
}

// complete this
int HashTable::probe(int index, int i) const {
    switch (method) {
        case LINEAR_PROBING:
            return (index + i) % tableSize;
        case QUADRATIC_PROBING:
            return (index + i*i) % tableSize;
        case DOUBLE_HASHING:
            // We'll handle double hashing directly in the main methods
            return index;
        default:
            return index;
    }
}

void HashTable::insert(const string& key, int value) {
    if (!isRehashing) {
        resizeIfNeeded();
    }
    
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR:
            for (auto& pair : tableVector[index]) {
                if (pair.first == key) {
                    pair.second = value; // Key already exists, update the value
                    return;
                }
            }
            tableVector[index].push_back({key, value});
            break;

        case CHAINING_LIST:
            for (auto& pair : tableList[index]) {
                if (pair.first == key) {
                    pair.second = value; // Key already exists, update the value
                    return;
                }
            }
            tableList[index].push_back({key, value});
            break;

        case CHAINING_BST:
            tableBST[index].insert(key, value);
            break;

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            int i = 0;
            int probeIndex;
            
            do {
                if (method == DOUBLE_HASHING) {
                    probeIndex = (index + i * hash2(key)) % tableSize;
                } else {
                    probeIndex = probe(index, i);
                }
                
                // If we find the key, update its value
                if (!tableProbing[probeIndex].first.empty() && 
                    tableProbing[probeIndex].first == key) {
                    tableProbing[probeIndex].second = value;
                    return;
                }
                
                // If we find an empty slot, insert the new key-value pair
                if (tableProbing[probeIndex].first.empty() || 
                    tableProbing[probeIndex].second == -1) { // Check for tombstone
                    tableProbing[probeIndex] = {key, value};
                    elementCount++;
                    return;
                }
                
                i++;
            } while (i < tableSize); // Prevent infinite loop
            
            // If we get here, the table is full (should not happen with resizeIfNeeded)
            return;
    }
    elementCount++;
}


bool HashTable::search(const string& key, int& value) {
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR:
            for (const auto& pair : tableVector[index]) {
                if (pair.first == key) {
                    value = pair.second;
                    return true;
                }
            }
            return false;

        case CHAINING_LIST:
            for (const auto& pair : tableList[index]) {
                if (pair.first == key) {
                    value = pair.second;
                    return true;
                }
            }
            return false;

        case CHAINING_BST:
            return tableBST[index].search(key, value);

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            int i = 0;
            int probeIndex;
            
            do {
                if (method == DOUBLE_HASHING) {
                    probeIndex = (index + i * hash2(key)) % tableSize;
                } else {
                    probeIndex = probe(index, i);
                }
                
                // If empty slot is found, the key is not in the table
                if (tableProbing[probeIndex].first.empty()) {
                    return false;
                }
                
                // Skip tombstones during search
                if (tableProbing[probeIndex].second == -1) {
                    i++;
                    continue;
                }
                
                // If the key is found, return its value
                if (tableProbing[probeIndex].first == key) {
                    value = tableProbing[probeIndex].second;
                    return true;
                }
                
                i++;
            } while (i < tableSize); // Prevent infinite loop
            
            return false;
    }
    
    return false; // Default return (should not reach here)
}
bool HashTable::remove(const string& key) {
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR: {
            auto& bucket = tableVector[index];
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key) {
                    bucket.erase(it);
                    elementCount--;
                    return true;
                }
            }
            return false;
        }

        case CHAINING_LIST: {
            auto& bucket = tableList[index];
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key) {
                    bucket.erase(it);
                    elementCount--;
                    return true;
                }
            }
            return false;
        }

        case CHAINING_BST: {
            bool removed = tableBST[index].remove(key);
            if (removed) {
                elementCount--;
                return true;
            }
            return false;
        }

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING: {
            int i = 0;
            int probeIndex;
            
            do {
                if (method == DOUBLE_HASHING) {
                    probeIndex = (index + i * hash2(key)) % tableSize;
                } else {
                    probeIndex = probe(index, i);
                }
                
                // If empty slot is found, the key is not in the table
                if (tableProbing[probeIndex].first.empty() && 
                    tableProbing[probeIndex].second != -1) { // Skip tombstones
                    return false;
                }
                
                // If the key is found, mark it as deleted
                if (tableProbing[probeIndex].first == key) {
                    // For open addressing, we use a "tombstone" approach
                    tableProbing[probeIndex].first = "";  // Empty string as tombstone
                    tableProbing[probeIndex].second = -1; // Special value to mark tombstone
                    elementCount--;
                    return true;
                }
                
                i++;
            } while (i < tableSize); // Prevent infinite loop
            
            return false;
        }
    }
    
    return false; // Default return (should not reach here)
}


// Function to read data from file
vector<pair<string, int>> readDataFromFile(const string& filename) {
    vector<pair<string, int>> data;
    ifstream file(filename);
    string key;
    int value;
    if (!file) {
        cerr << "Error reading file: " << filename << endl;
        return data;
    }
    while (file >> key >> value) {
        data.push_back({key, value});
    }

    file.close();
    return data;
}

// Benchmark function for custom HashTable
void HashTable::benchmarkHashTable(HashTable& table, const vector<pair<string, int>>& data, int numSearch, int numDelete) {
    auto start = chrono::high_resolution_clock::now();

    // Insertions
    for (const auto& pair : data) {
        table.insert(pair.first, pair.second);
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Searches
    int value;
    for (int i = 0; i < numSearch; ++i) {
        table.search(data[rand() % data.size()].first, value);
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Deletions
    for (int i = 0; i < numDelete; ++i) {
        table.remove(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " << deleteTime.count() << " microseconds" << endl;
}

void HashTable::displayStats() {
    int totalElements = 0;
    int maxChainLength = 0;
    int emptyBuckets = 0;

    switch (method) {
        case CHAINING_VECTOR:
            for (const auto& bucket : tableVector) {
                if (bucket.empty()) emptyBuckets++;
                maxChainLength = max(maxChainLength, (int)bucket.size());
                totalElements += bucket.size();
            }
            break;
        case CHAINING_LIST:
            for (const auto& bucket : tableList) {
                if (bucket.empty()) emptyBuckets++;
                maxChainLength = max(maxChainLength, (int)bucket.size());
                totalElements += bucket.size();
            }
            break;
        case CHAINING_BST:
            for (const auto& bucket : tableBST) {
                vector<pair<string, int>> elements = bucket.inOrderTraversal();
                if (elements.empty()) {
                    emptyBuckets++;
                } else {
                    maxChainLength = max(maxChainLength, (int)elements.size());
                    totalElements += elements.size();
                }
            }
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            for (const auto& entry : tableProbing) {
                if (!entry.first.empty()) totalElements++;
                else emptyBuckets++;
            }
            break;
    }

    cout << "Total elements: " << totalElements << endl;
    cout << "Load factor: " << (double)totalElements / tableSize << endl;
    cout << "Empty buckets: " << emptyBuckets << endl;
    if (method == CHAINING_VECTOR || method == CHAINING_LIST || method == CHAINING_BST) {
        cout << "Max chain length: " << maxChainLength << endl;
    }
}


void HashTable::rehash() {
    // complete this
}


int HashTable::findEmptySlot(const string& key) {
    // complete this
}

// try experimenting with different thresholds for each technique
void HashTable::resizeIfNeeded() {
    // complete this
}


void HashTable::rehash() {
    isRehashing = true;
    int oldSize = tableSize;
    tableSize = oldSize * 2;
    
    switch (method) {
        case CHAINING_VECTOR: {
            vector<vector<pair<string, int>>> oldTable = tableVector;
            tableVector.clear();
            tableVector.resize(tableSize);
            
            elementCount = 0;
            for (const auto& bucket : oldTable) {
                for (const auto& pair : bucket) {
                    insert(pair.first, pair.second);
                }
            }
            break;
        }
        
        case CHAINING_LIST: {
            vector<list<pair<string, int>>> oldTable = tableList;
            tableList.clear();
            tableList.resize(tableSize);
            
            elementCount = 0;
            for (const auto& bucket : oldTable) {
                for (const auto& pair : bucket) {
                    insert(pair.first, pair.second);
                }
            }
            break;
        }
        
        case CHAINING_BST: {
            vector<AVLTree> oldTable = tableBST;
            tableBST.clear();
            tableBST.resize(tableSize);
            
            elementCount = 0;
            for (auto& tree : oldTable) {
                vector<pair<string, int>> elements = tree.inOrderTraversal();
                for (const auto& pair : elements) {
                    insert(pair.first, pair.second);
                }
            }
            break;
        }
        
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING: {
            vector<pair<string, int>> oldTable = tableProbing;
            tableProbing.clear();
            tableProbing.resize(tableSize);
            
            elementCount = 0;
            for (const auto& pair : oldTable) {
                if (!pair.first.empty() && pair.second != -1) {
                    insert(pair.first, pair.second);
                }
            }
            break;
        }
    }
    isRehashing = false;
}
int HashTable::findEmptySlot(const string& key) {
    int index = hash1(key);
    int i = 0;
    int probeIndex;
    
    do {
        if (method == DOUBLE_HASHING) {
            probeIndex = (index + i * hash2(key)) % tableSize;
        } else {
            probeIndex = probe(index, i);
        }
        
        // Found an empty slot, a tombstone, or the same key
        if (tableProbing[probeIndex].first.empty() || 
            tableProbing[probeIndex].second == -1 ||
            tableProbing[probeIndex].first == key) {
            return probeIndex;
        }
        
        i++;
    } while (i < tableSize);
    
    return -1;  // Table is full (should not happen with resizeIfNeeded)
}
// try experimenting with different thresholds for each technique
void HashTable::resizeIfNeeded() {
    double loadFactor = static_cast<double>(elementCount) / tableSize;
    
    double threshold;
    
    switch (method) {
        case CHAINING_VECTOR:
        case CHAINING_LIST:
        case CHAINING_BST:
            threshold = 0.75;  // Higher threshold for chaining (can handle more collisions)
            break;
        case LINEAR_PROBING:
            threshold = 0.5;   // Lower threshold for linear probing (more sensitive to clustering)
            break;
        case QUADRATIC_PROBING:
            threshold = 0.55;  // Slightly higher than linear probing
            break;
        case DOUBLE_HASHING:
            threshold = 0.6;   // Higher than quadratic probing (better distribution)
            break;
        default:
            threshold = 0.75;
    }
    
    if (loadFactor >= threshold) {
        rehash();
    }
}    


// Benchmark function for built-in hash table in C++
void benchmarkStdUnorderedMap(const vector<pair<string, int>>& data, int numSearch, int numDelete) {
    unordered_map<string, int> stdMap;

    auto start = chrono::high_resolution_clock::now();

    // Insertions
    for (const auto& pair : data) {
        stdMap[pair.first] = pair.second;
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Searches
    int value;
    for (int i = 0; i < numSearch; ++i) {
        auto it = stdMap.find(data[rand() % data.size()].first);
        if (it != stdMap.end()) {
            value = it->second;
        }
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Deletions
    for (int i = 0; i < numDelete; ++i) {
        stdMap.erase(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "std::unordered_map Benchmark Results:" << endl;
    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " <
