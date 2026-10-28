#pragma once
#include <iostream>
#include "Deque.h"

template<typename T>
class Queue
{
private:
    Deque<T> data;

public:
    Queue();

    bool empty() const;
    void clear();
    int size() const;

    void EnQueue(const T& value);
    void DeQueue();

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;
};

template<typename T>
Queue<T>::Queue() : data() {}

template<typename T>
bool Queue<T>::empty() const {
    return data.empty();
}

template<typename T>
void Queue<T>::clear() {
    data.clear();
}

template<typename T>
int Queue<T>::size() const {
    return data.size();
}

template<typename T>
void Queue<T>::EnQueue(const T& value) {
    data.push_back(value);
}

template<typename T>
void Queue<T>::DeQueue() {
    data.pop_front();
}

template<typename T>
T& Queue<T>::front() {
    return data.front();
}

template<typename T>
const T& Queue<T>::front() const {
    return data.front();
}

template<typename T>
T& Queue<T>::back() {
    return data.back();
}

template<typename T>
const T& Queue<T>::back() const {
    return data.back();
}