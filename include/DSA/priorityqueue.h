#pragma once
#include <stdexcept>
#include <utility>
#include <functional>
#include "vector.h"

template<typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    Vector<T> heap;
    Compare comp;

    void heapifyUp(int index) {
        while (index > 0) {
            int p = (index - 1) / 2;
            if (comp(heap[p], heap[index])) {
                std::swap(heap[p], heap[index]);
                index = p;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int n = heap.size();
        while (2 * index + 1 < n) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < n && comp(heap[largest], heap[left])) {
                largest = left;
            }
            if (right < n && comp(heap[largest], heap[right])) {
                largest = right;
            }
            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    PriorityQueue() : heap(), comp(Compare()) {}
    explicit PriorityQueue(const Compare& comparator) : heap(), comp(comparator) {}

    bool empty() const noexcept {
        return heap.empty();
    }

    int size() const noexcept {
        return heap.size();
    }

    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    void push(T&& value) {
        heap.push_back(std::move(value));
        heapifyUp(heap.size() - 1);
    }

    const T& top() const {
        if (heap.empty()) {
            throw std::out_of_range("PriorityQueue is empty!");
        }
        return heap[0];
    }

    void pop() {
        if (heap.empty()) {
            throw std::out_of_range("PriorityQueue is empty!");
        }
        heap[0] = std::move(heap.back());
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    void clear() noexcept {
        heap.clear();
    }

    void swap(PriorityQueue& other) noexcept {
        heap.swap(other.heap);
        std::swap(comp, other.comp);
    }
};

// Backwards compatibility alias
template<typename T, typename Compare = std::less<T>>
using Priorityqueue = PriorityQueue<T, Compare>;