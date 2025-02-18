#include "splay.h"
#include "customErrorClass.h"
#include <iostream>
 
using namespace std;
 
int main() {
    SplayTree tree;
 
    try {
        tree.insert(57);
        tree.insert(31);
        tree.insert(72);
        tree.insert(44);
        tree.insert(69);
        tree.insert(83);
 
        cout << "Tree after insertions: " << endl;
        tree.display();
 
        if (!tree.search(44)) {
            throw MyException("Error: Key 44 not found after splay operation.");
        }
 
        cout << "Tree after splaying 44: " << endl;
        tree.display();
 
        tree.remove(31);
        cout << "Tree after deleting 31: " << endl;
        tree.display();
 
        // Trying to remove a non-existent key
        tree.remove(100);
 
    } catch (MyException &e) {
        cerr << "Exception: " << e.what() << endl;
    } catch (exception &e) {
        cerr << "Standard exception: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown error occurred!" << endl;
    }
 
    return 0;
}
 