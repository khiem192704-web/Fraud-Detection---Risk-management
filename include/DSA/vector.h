#pragma once
#include <iostream>
#include <stdexcept>
#include <algorithm>

template<typename T>
class Vector
{
private:
    T* data;
    int size_;
    int capacity_;

    void grow();

public:
    Vector();
    explicit Vector(int initSize);
    Vector(const Vector& other);
    ~Vector();

    Vector<T>& operator=(const Vector& other);
    T& operator[](int index);
    const T& operator[](int index) const;

    bool empty() const;
    int size() const;
    int capacity() const;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    T* begin();
    const T* begin() const;
    T* end();
    const T* end() const;

    void push_back(const T& value);
    void insert(int index, const T& value);
    void pop_back();
    void erase(int index);
    void clear();

    void reserve(int new_capacity_);
    void resize(int new_size_);
    void fit_capacity();
};

template<typename T>
Vector<T>::Vector() : data(nullptr), size_(0), capacity_(0) {}

template<typename T>
Vector<T>::Vector(int initSize) : size_(initSize < 0 ? 0 : initSize), capacity_(initSize < 0 ? 0 : initSize) {
    if (this->capacity_ > 0) {
        this->data = new T[this->capacity_];
        for (int i = 0; i < this->size_; i++) {
            this->data[i] = T();
        }
    } else {
        this->data = nullptr;
    }
}

template<typename T>
Vector<T>::Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_) {
    if (other.data == nullptr || other.capacity_ == 0) {
        this->data = nullptr;
        return;
    }
    this->data = new T[this->capacity_];
    for (int i = 0; i < size_; i++) {
        this->data[i] = other.data[i];
    }
}

template<typename T>
Vector<T>::~Vector() {
    if (this->data != nullptr) {
        delete[] this->data;
    }
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        delete[] this->data;
        this->size_ = other.size_;
        this->capacity_ = other.capacity_;
        if (other.data == nullptr || other.capacity_ == 0) {
            this->data = nullptr;
        } else {
            this->data = new T[this->capacity_];
            for (int i = 0; i < size_; i++) {
                this->data[i] = other.data[i];
            }
        }
    }
    return *this;
}

template<typename T>
T& Vector<T>::operator[](int index) {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Vector index out of range");
    }
    return this->data[index];
}

template<typename T>
const T& Vector<T>::operator[](int index) const {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Vector index out of range");
    }
    return this->data[index];
}

template<typename T>
bool Vector<T>::empty() const {
    return this->size_ == 0;
}

template<typename T>
int Vector<T>::size() const {
    return this->size_;
}

template<typename T>
int Vector<T>::capacity() const {
    return this->capacity_;
}

template<typename T>
T& Vector<T>::front() {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return this->data[0];
}

template<typename T>
const T& Vector<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return this->data[0];
}

template<typename T>
T& Vector<T>::back() {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return this->data[this->size_ - 1];
}

template<typename T>
const T& Vector<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return this->data[this->size_ - 1];
}

template<typename T>
T* Vector<T>::begin() {
    return this->data;
}

template<typename T>
const T* Vector<T>::begin() const {
    return this->data;
}

template<typename T>
T* Vector<T>::end() {
    return this->data + this->size_;
}

template<typename T>
const T* Vector<T>::end() const {
    return this->data + this->size_;
}

template<typename T>
void Vector<T>::grow() {
    reserve((capacity_ == 0) ? 1 : capacity_ * 2);
}

template<typename T>
void Vector<T>::push_back(const T& value) {
    if (this->size_ == this->capacity_) {
        T val = value;
        grow();
        this->data[this->size_++] = val;
    } else {
        this->data[this->size_++] = value;
    }
}

template<typename T>
void Vector<T>::insert(int index, const T& value) {
    if (index < 0 || index > this->size_) {
        throw std::out_of_range("Insert index out of range");
    }
    T val = value;
    if (this->size_ == this->capacity_) grow();
    for (int i = this->size_; i > index; i--) {
        this->data[i] = this->data[i - 1];
    }
    this->data[index] = val;
    this->size_++;
}

template<typename T>
void Vector<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    this->size_--;
    if (size_ > 0 && size_ <= capacity_ / 4) {
        reserve(capacity_ / 2);
    }
}

template<typename T>
void Vector<T>::erase(int index) {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Iterator out of range");
    }
    for (int i = index; i < this->size_ - 1; i++) {
        this->data[i] = this->data[i + 1];
    }
    this->size_--;
    if (size_ > 0 && size_ <= capacity_ / 4) {
        reserve(capacity_ / 2);
    }
}

template<typename T>
void Vector<T>::clear() {
    this->size_ = 0;
}

template<typename T>
void Vector<T>::reserve(int new_capacity_) {
    if (new_capacity_ < this->size_ || new_capacity_ == this->capacity_) return;
    T* new_data = new T[new_capacity_];
    for (int i = 0; i < size_; i++) {
        new_data[i] = this->data[i];
    }
    delete[] this->data;
    this->data = new_data;
    this->capacity_ = new_capacity_;
}

template<typename T>
void Vector<T>::resize(int new_size_) {
    if (new_size_ < 0) return;
    if (new_size_ > this->capacity_) {
        reserve(new_size_);
    }
    for (int i = this->size_; i < new_size_; i++) {
        this->data[i] = T();
    }
    this->size_ = new_size_;
}

template<typename T>
void Vector<T>::fit_capacity() {
    if (this->capacity_ <= this->size_) return;
    if (this->size_ == 0) {
        delete[] this->data;
        this->data = nullptr;
        this->capacity_ = 0;
        return;
    }
    T* new_data = new T[this->size_];
    for (int i = 0; i < this->size_; i++) {
        new_data[i] = this->data[i];
    }
    delete[] this->data;
    this->data = new_data;
    this->capacity_ = this->size_;
}