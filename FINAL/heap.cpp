#include "heap.h"
#include <iostream>

using namespace std;

MinHeap::MinHeap(int capacity) : capacity(capacity), size(0) {
    arr = new HeapNode[capacity];
}

MinHeap::~MinHeap() {
    delete[] arr;
}

void MinHeap::swap(HeapNode& a, HeapNode& b) {
    HeapNode temp = a;
    a = b;
    b = temp;
}

void MinHeap::heapifyUp(int idx) {
    while (idx > 0 && arr[(idx-1)/2].distance > arr[idx].distance) {
        swap(arr[(idx-1)/2], arr[idx]);
        idx = (idx-1)/2;
    }
}

void MinHeap::heapifyDown(int idx) {
    int smallest = idx;
    int left = 2*idx + 1;
    int right = 2*idx + 2;

    if (left < size && arr[left].distance < arr[smallest].distance)
        smallest = left;
    if (right < size && arr[right].distance < arr[smallest].distance)
        smallest = right;
    if (smallest != idx) {
        swap(arr[idx], arr[smallest]);
        heapifyDown(smallest);
    }
}

void MinHeap::insert(int x, int y, int distance) {
    if (size == capacity) return;
    arr[size] = HeapNode(x, y, distance);
    heapifyUp(size);
    size++;
}

HeapNode MinHeap::extractMin() {
    if (size <= 0) return HeapNode(-1, -1, -1);
    HeapNode min = arr[0];
    arr[0] = arr[size - 1];
    size--;
    heapifyDown(0);
    return min;
}
