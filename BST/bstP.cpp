#include "bstP.h"

node::node(int value)
{
    data = value;
    leftChild = nullptr;
    rightChild = nullptr;
}

BST::BST()
{
    root = nullptr;
}

BST::~BST()
{
    clearTree(root);
    // or call deleteNode
}

void BST::clearTree(node* node)
{
    if (root == nullptr)
        return;

    clearTree(node->leftChild);
    clearTree(node->rightChild);
    delete node;
}

void BST:: insertNode(node*& root, int data)
{
    // complete this
    if(root == nullptr){
        root = new node (data);
        return;
    }

    if(data < root -> data)
    insertNode(root ->leftChild, data);
    else
    insertNode(root ->rightChild, data);
}

node* BST:: searchNode(node*& root, int data)
{
    if(root == nullptr){
        cout << data << "Not found" << endl;
        return nullptr;
    }

    if(root -> data == data){
        cout << data << "found" << endl;
        return root;
    }

    if(data < root -> data)
    return searchNode(root -> leftChild, data);
    else
    return searchNode(root -> rightChild, data);

}

void BST:: deleteNode(node*& root, int data)
{
    // complete this
   
    
if(root == nullptr){ 
cout << data << " was not found. Can't delete" << endl;
 return;
}

if(data < root -> data)

deleteNode(root -> leftChild, data);

else if (data > root -> data)

deleteNode(root->rightChild, data);
else { 
// Case 1 - no child or one child

if (root->leftChild = nullptr) {

node* replacement = root->rightChild;

delete root;

root = replacement;

} else if (root -> rightChild = nullptr) {

node * replacement = root->leftChild;

delete root;

root = replacement;}

else  { // Case 3 - two children


int successor = SmallNode(root->rightChild);

root -> data = successor;

deleteNode(root -> rightChild, successor);
}
}
}



void BST::preorder(node* root)
{
    // complete this
    if (root == nullptr)
    return;

    cout << root -> data << " ";
    preorder (root -> leftChild);
    preorder (root -> rightChild);

}
void BST::inorder(node* root)
{
    // complete this
    if(root == nullptr)
    return;

     inorder (root -> rightChild);
    cout << root -> data << " ";
    inorder (root -> rightChild);
}
void BST::postorder(node* root)
{
    // complete this
    
    postorder (root -> leftChild);
    postorder (root -> rightChild);
     cout << root -> data << " ";
}

int BST::height(node* node)
{
    // complete this
    if(node == nullptr)
    return 0;

    int leftHeight = height(node -> leftChild);
    int rightHeight = height(node -> rightChild);

    return 1 + max (leftHeight, rightHeight);
}

bool BST:: isBalanced(node* root)
{
    // complete this
    if (root == nullptr)
    return true;

    int leftHeight = height(root -> leftChild);
    int rightHeight = height(root -> rightChild);

    if(abs(leftHeight - rightHeight) > 1)
    return false;

    return isBalanced (root -> leftChild) && isBalanced(root -> rightChild);

}

void BST::printTree(node* root, int space) {
    const int COUNT = 10; 

    if (root == nullptr) {
        return;
    }

    // Increase the distance between levels
    space += COUNT;

    // Print the right child first (to appear on top)
    printTree(root->rightChild, space);

    // Print the current node after right child

    for (int i = COUNT; i < space; i++) {
        cout << " "; // Indentation for tree depth
    }
    cout << root->data << endl;

    // Print the left child
    printTree(root->leftChild, space);
}

