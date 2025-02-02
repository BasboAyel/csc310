#include "bstP.h"

// Feel free to modify this

int main (int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: ./a <filename>" << endl;
        return 1;
    }

    string filename = argv[1];

    BST tree;

    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file.\n";
        return 1;
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        int command, data;

        iss >> command;

        switch (command) {
            case 0:
                cout << "Exiting...!\n";
                return 0;
            case 1:
                iss >> data;
                tree.insertNode(tree.root, data);
                cout << "Insert: " << data << endl;
                break;
            case 2:
                iss >> data;
                tree.searchNode(tree.root, data);
                break;
            case 3:
                iss >> data;
                tree.deleteNode(tree.root, data);
                break;
            case 4:
                cout << "Inorder traversal: ";
                tree.inorder(tree.root);
                cout << endl;
                break;
            case 5:
                cout << "Preorder traversal: ";
                tree.preorder(tree.root);
                cout << endl;
                break;
            case 6:
                cout << "Postorder traversal: ";
                tree.postorder(tree.root);
                cout << endl;
                break;
            case 7:
                cout << "Binary Search Tree" << endl;
                tree.printTree(tree.root, 0);
                cout<<endl;
                break;
            default:
                cout << "Invalid command." << endl;
        }
    }


    return 0;
}