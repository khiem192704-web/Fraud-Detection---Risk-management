#pragma once

#include <stdexcept>
#include "vector.h"

template<typename T>
class PriorityQueue {
private:
    Vector<T> heap;

    void swap(T& a, T& b);
    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    PriorityQueue();

    bool empty() const;
    int size() const;

    void push(const T& value);
    const T& top() const;
    void pop();
    void clear();
};

template<typename T>
PriorityQueue<T>::PriorityQueue() : heap() {}

template<typename T>
void PriorityQueue<T>::swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
void PriorityQueue<T>::heapifyUp(int index) {
    while (index > 0) {
        int p = (index - 1) / 2;
        if (heap[p] < heap[index]) {
            swap(heap[p], heap[index]);
            index = p;
        } else {
            break;
        }
    }
}

template<typename T>
void PriorityQueue<T>::heapifyDown(int index) {
    int n = heap.size();
    while (2 * index + 1 < n) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < n && heap[largest] < heap[left]) largest = left;
        if (right < n && heap[largest] < heap[right]) largest = right;
        if (largest != index) {
            swap(heap[index], heap[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

template<typename T>
bool PriorityQueue<T>::empty() const {
    return heap.empty();
}

template<typename T>
int PriorityQueue<T>::size() const {
    return heap.size();
}

template<typename T>
void PriorityQueue<T>::push(const T& value) {
    heap.push_back(value);
    heapifyUp(heap.size() - 1);
}

template<typename T>
const T& PriorityQueue<T>::top() const {
    if (heap.empty()) throw std::out_of_range("Priorityqueue is empty!");
    return heap[0];
}

template<typename T>
void PriorityQueue<T>::pop() {
    if (heap.empty()) return;
    swap(heap[0], heap.back());
    heap.pop_back();
    if (!heap.empty()) heapifyDown(0);
}

template<typename T>
void PriorityQueue<T>::clear() {
    heap.clear();
}