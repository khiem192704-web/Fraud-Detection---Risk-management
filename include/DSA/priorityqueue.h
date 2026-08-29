#pragma once

#include<stdexcept>
#include"include/DSA/Vector.h"

template<typename T>
class Priorityqueue{
    private:
        Vector<T> heap;
        void swap(T& a, T& b) noexcept {
            T temp = a;
            a = b;
            b = temp;
        }
        void heapifyUp (int index){
            while(index > 0){
                int p = (index - 1)/2;
                if(heap[p] < heap[index]){
                    swap(heap[p], heap[index]);
                    index = p;
                }
                else break;
            }
        }
        void heapifyDown(int index){
            int n = heap.size();
            while(2*index + 1 < n){
                int left = 2*index + 1;
                int right = 2*index + 2;
                int largest = index]

                if(left < n && heap[largest] < heap[left]) largest = left;
                if(right < n && heap[largest] < heap[right]) largest = right;
                if(largest != index){
                    swap(heap[index], heap[largest]);
                    index = largest;
                }
                else break;
            }
        }
    public:
        Priorityqueue() = default;

        bool empty() const noexcept {
            return heap.empty();
        }

        int size() const noexcept {
            return heap.size();
        }

        void push(const T& value){
            heap.push_back(value);
            heapifyUp(heap.size() - 1);
        }
        const T& top() const {
            if(heap.empty()) throw out_of_range("Priorityqueue is empty!");
            return heap[0];
        }
        void pop(){
            if(heap.empty()) return;
            heap[0] = heap.back();
            heap.pop_back();
            if(!heap.empty()) heapifyDown(0);
        }
        void clear() noexpect {
            heap.clear();
        }
};