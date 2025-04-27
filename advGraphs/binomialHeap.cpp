#include "binomialHeap.h"
#include <climits>

// Constructor for binomialNode
binomialNode::binomialNode(Edge e)
{
    edge = e;
    degree = 0;
    parent = nullptr;
    sibling = nullptr;
    child = nullptr;
}

// ---------------------- PRIVATE FUNCTIONS ------------------------------

binomialNode* BinomialHeap::unionHeaps(binomialNode* heap1, binomialNode* heap2)
{
    if (!heap1) return heap2;
    if (!heap2) return heap1;

    binomialNode* newHead = nullptr;
    binomialNode** pos = &newHead;

    while (heap1 && heap2)
    {
        if (heap1->degree <= heap2->degree)
        {
            *pos = heap1;
            heap1 = heap1->sibling;
        }
        else
        {
            *pos = heap2;
            heap2 = heap2->sibling;
        }
        pos = &((*pos)->sibling);
    }

    *pos = (heap1) ? heap1 : heap2;
    return newHead;
}

binomialNode* BinomialHeap::mergeTrees(binomialNode* tree1, binomialNode* tree2)
{
    if (tree1->edge.weight > tree2->edge.weight)
        swap(tree1, tree2);

    tree2->parent = tree1;
    tree2->sibling = tree1->child;
    tree1->child = tree2;
    tree1->degree++;

    return tree1;
}

void BinomialHeap::linkTrees(binomialNode*& prev, binomialNode*& cur, binomialNode*& next)
{
    if (cur->degree != next->degree || (next->sibling && next->sibling->degree == cur->degree))
    {
        prev = cur;
        cur = next;
    }
    else
    {
        if (cur->edge.weight <= next->edge.weight)
        {
            cur->sibling = next->sibling;
            mergeTrees(cur, next);
        }
        else
        {
            if (!prev)
                head = next;
            else
                prev->sibling = next;
            mergeTrees(next, cur);
            cur = next;
        }
    }
}

// ---------------------- PUBLIC FUNCTIONS ------------------------------

BinomialHeap::BinomialHeap()
{
    head = nullptr;
}

void BinomialHeap::insert(Edge e)
{
    BinomialHeap tempHeap;
    tempHeap.head = new binomialNode(e);

    head = unionHeaps(head, tempHeap.head);

    if (!head || !head->sibling)
        return;

    binomialNode* prev = nullptr;
    binomialNode* cur = head;
    binomialNode* next = cur->sibling;

    while (next)
    {
        linkTrees(prev, cur, next);
        next = cur->sibling;
    }
}

void BinomialHeap::merge(BinomialHeap& other)
{
    head = unionHeaps(head, other.head);
    other.head = nullptr;

    if (!head || !head->sibling)
        return;

    binomialNode* prev = nullptr;
    binomialNode* cur = head;
    binomialNode* next = cur->sibling;

    while (next)
    {
        linkTrees(prev, cur, next);
        next = cur->sibling;
    }
}

Edge BinomialHeap::findMin()
{
    if (!head)
        return Edge{-1, -1, -1};

    binomialNode* minNode = head;
    int minWeight = head->edge.weight;

    binomialNode* temp = head->sibling;
    while (temp)
    {
        if (temp->edge.weight < minWeight)
        {
            minWeight = temp->edge.weight;
            minNode = temp;
        }
        temp = temp->sibling;
    }

    return minNode->edge;
}

void BinomialHeap::deleteMin()
{
    if (!head)
        return;

    binomialNode* minNode = head;
    binomialNode* minPrev = nullptr;
    binomialNode* prev = nullptr;
    binomialNode* curr = head;
    int minWeight = head->edge.weight;

    while (curr)
    {
        if (curr->edge.weight < minWeight)
        {
            minWeight = curr->edge.weight;
            minNode = curr;
            minPrev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }

    // Remove minNode from root list
    if (minPrev)
        minPrev->sibling = minNode->sibling;
    else
        head = minNode->sibling;

    // Reverse minNode's children
    binomialNode* child = minNode->child;
    binomialNode* reversed = nullptr;

    while (child)
    {
        binomialNode* next = child->sibling;
        child->sibling = reversed;
        child->parent = nullptr;
        reversed = child;
        child = next;
    }

    BinomialHeap tempHeap;
    tempHeap.head = reversed;
    head = unionHeaps(head, tempHeap.head);

    if (!head || !head->sibling)
    {
        delete minNode;
        return;
    }

    binomialNode* prevLink = nullptr;
    binomialNode* curLink = head;
    binomialNode* nextLink = curLink->sibling;

    while (nextLink)
    {
        linkTrees(prevLink, curLink, nextLink);
        nextLink = curLink->sibling;
    }

    delete minNode;
}

// Helper to find a node with a given Edge
binomialNode* BinomialHeap::findNode(binomialNode* root, Edge edge)
{
    if (!root) return nullptr;
    if (root->edge.src == edge.src && root->edge.dest == edge.dest && root->edge.weight == edge.weight)
        return root;

    binomialNode* found = findNode(root->child, edge);
    if (found) return found;

    return findNode(root->sibling, edge);
}

void BinomialHeap::decreaseKey(Edge oldEdge, Edge newEdge)
{
    if (!head || newEdge.weight > oldEdge.weight)
        return;

    binomialNode* target = findNode(head, oldEdge);
    if (!target) return;

    target->edge = newEdge;

    while (target->parent && target->edge.weight < target->parent->edge.weight)
    {
        swap(target->edge, target->parent->edge);
        target = target->parent;
    }
}

void BinomialHeap::deleteKey(Edge edge)
{
    decreaseKey(edge, Edge{-1, -1, INT_MIN});
    deleteMin();
}

void BinomialHeap::printHeap()
{
    cout << "Binomial Heap:\n";
    binomialNode* current = head;

    while (current != nullptr)
    {
        cout << endl << "Tree of degree " << current->degree << endl;
        printTree(current, 0);
        current = current->sibling;
    }
}

void BinomialHeap::printTree(binomialNode* node, int space)
{
    if (node == nullptr)
        return;

    cout << setw(space * 2) << "(" << node->edge.src << ", " << node->edge.dest << ", " << node->edge.weight << ")\n";

    binomialNode* child = node->child;
    while (child)
    {
        printTree(child, space + 3);
        child = child->sibling;
    }
}
