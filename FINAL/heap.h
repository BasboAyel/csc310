#ifndef HEAP_H
#define HEAP_H

struct HeapNode {
    int x;
    int y;
    int distance;

    HeapNode(int x = 0, int y = 0, int d = 0) : x(x), y(y), distance(d) {}
};

class MinHeap {
private:
    HeapNode* arr;
    int size;
    int capacity;
    void heapifyUp(int idx);
    void heapifyDown(int idx);
    void swap(HeapNode& a, HeapNode& b);
public:
    MinHeap(int capacity);
    ~MinHeap();
    void insert(int x, int y, int distance);
    HeapNode extractMin();
};

#endif
