#pragma once
#include <stdexcept>

template<typename T>
class Deque
{
private:
    T* data;
    int capacity;
    int frontIndex;
    int count;

    void resize(int newCapacity);
    void shrinkIfNeeded();

public:
    Deque();
    Deque(const Deque& other);
    Deque& operator=(const Deque& other);
    ~Deque();

    bool empty() const;
    int size() const;

    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back();
    void pop_front();

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    T& operator[](int index);
    const T& operator[](int index) const;

    void clear();
};

template<typename T>
void Deque<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];
    for (int i = 0; i < count; i++) {
        newData[i] = data[(frontIndex + i) % capacity];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
    frontIndex = 0;
}

template<typename T>
void Deque<T>::shrinkIfNeeded() {
    if (count > 0 && count <= capacity / 4 && capacity > 8) {
        resize(capacity / 2);
    }
}

template<typename T>
Deque<T>::Deque() : capacity(8), frontIndex(0), count(0) {
    data = new T[capacity];
}

template<typename T>
Deque<T>::Deque(const Deque& other) : capacity(other.capacity), frontIndex(0), count(other.count) {
    data = new T[capacity];
    for (int i = 0; i < count; i++) {
        data[i] = other.data[(other.frontIndex + i) % other.capacity];
    }
}

template<typename T>
Deque<T>& Deque<T>::operator=(const Deque& other) {
    if (this != &other) {
        T* newData = new T[other.capacity];
        for (int i = 0; i < other.count; i++) {
            newData[i] = other.data[(other.frontIndex + i) % other.capacity];
        }
        delete[] data;
        data = newData;
        capacity = other.capacity;
        count = other.count;
        frontIndex = 0;
    }
    return *this;
}

template<typename T>
Deque<T>::~Deque() {
    delete[] data;
}

template<typename T>
bool Deque<T>::empty() const {
    return count == 0;
}

template<typename T>
int Deque<T>::size() const {
    return count;
}

template<typename T>
void Deque<T>::push_back(const T& value) {
    if (count == capacity) {
        T val = value;
        resize(capacity * 2);
        data[(frontIndex + count) % capacity] = val;
    } else {
        data[(frontIndex + count) % capacity] = value;
    }
    count++;
}

template<typename T>
void Deque<T>::push_front(const T& value) {
    if (count == capacity) {
        T val = value;
        resize(capacity * 2);
        frontIndex = (frontIndex - 1 + capacity) % capacity;
        data[frontIndex] = val;
    } else {
        frontIndex = (frontIndex - 1 + capacity) % capacity;
        data[frontIndex] = value;
    }
    count++;
}

template<typename T>
void Deque<T>::pop_back() {
    if (empty()) return;
    count--;
    shrinkIfNeeded();
}

template<typename T>
void Deque<T>::pop_front() {
    if (empty()) return;
    frontIndex = (frontIndex + 1) % capacity;
    count--;
    shrinkIfNeeded();
}

template<typename T>
T& Deque<T>::front() {
    if (empty()) {
        throw std::out_of_range("Deque is empty");
    }
    return data[frontIndex];
}

template<typename T>
const T& Deque<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Deque is empty");
    }
    return data[frontIndex];
}

template<typename T>
T& Deque<T>::back() {
    if (empty()) {
        throw std::out_of_range("Deque is empty");
    }
    return data[(frontIndex + count - 1) % capacity];
}

template<typename T>
const T& Deque<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Deque is empty");
    }
    return data[(frontIndex + count - 1) % capacity];
}

template<typename T>
T& Deque<T>::operator[](int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index out of range");
    }
    return data[(frontIndex + index) % capacity];
}

template<typename T>
const T& Deque<T>::operator[](int index) const {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index out of range");
    }
    return data[(frontIndex + index) % capacity];
}

template<typename T>
void Deque<T>::clear() {
    count = 0;
    frontIndex = 0;
}