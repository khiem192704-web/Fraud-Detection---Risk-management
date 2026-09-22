#pragma once
#include<iostream>
#include <stdexcept>
#include <algorithm>

template<typename T>
class Vector
{
    private:
        T* data;
        int size_;
        int capacity_;
        void grow(){
            reserve((capacity_ == 0) ? 1 : capacity_ * 2);
        }
    public:
        Vector() : data(nullptr), size_(0), capacity_(0){}
        Vector(int initSize_) : size_(initSize_), capacity_(initSize_){
            data = new T[capacity_];
            for(int i = 0; i < size_; i++){
                data[i] = T();
            }
        }
        Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_){
            if(other.data == nullptr){
                data = nullptr;
                return;
            }
            data = new T[capacity_];
            for(int i = 0; i < size_; i++){
                data[i] = other.data[i];
            }
        }
        ~Vector(){  delete[] data; }
        //(a = b) = c
        Vector& operator=(const Vector& other) {
            if(this != &other){
                delete[] data;
                data = new T[other.capacity_];
                size_ = other.size_;
                capacity_ = other.capacity_;
                for(int i = 0; i < size_; i++){
                    data[i] = other.data[i];
                }
            }
            return *this;
        }
        T& operator[](int index) {
            return data[index];
        }
        const T& operator[](int index) const{
            return data[index];
        }
        // T& at(int index){
        //     if(index < 0 || index >= size_){ throw std::out_of_range("Vector index out of range");}
        //     return data[index];
        // }
        // const T& at(int index) const{
        //     if(index < 0 || index >= size_){ throw std::out_of_range("Vector index out of range");}
        //     return data[index];
        // }
        
        bool empty() const {
            return size_ == 0;
        }
        
        T& front(){
            if (empty()) {
                throw std::out_of_range("Vector is empty");
            }
            return data[0];
        }
        
        const T& front() const{
            if (empty()) {
                throw std::out_of_range("Vector is empty");
            }
            return data[0];
        }
        
        T& back(){
            if (empty()) {
                throw std::out_of_range("Vector is empty");
            }
            return data[size_ - 1];
        }
        
        const T& back() const{
            if (empty()) {
                throw std::out_of_range("Vector is empty");
            }
            return data[size_ - 1];
        }
        
        int size() const{
            return size_;
        }
        int capacity() const{
            return capacity_;
        }
        
        T* begin(){
            return data;
        }
        const T* begin() const {
            return data;
        }
        T* end(){
            return data + size_;
        }
        const T* end() const {
            return data + size_;
        }
        void push_back(const T& value){
            if(size_ == capacity_){
                grow();
            }
            data[size_++] = value;
        }
        
        void insert(int index, const T& value){
            if(index < 0 || index > size_){
                throw std::out_of_range("Insert index out of range");
            }
            if(size_ == capacity_) grow();
            for(int i = size_; i > index; i--) data[i] = data[i - 1];
            data[index] = value;
            size_++;
        }
        
        void pop_back(){
            if(empty()){
                throw std::out_of_range("Vector is empty");
            }
            size_--;
            if (size_ > 0 && size_ <= capacity_ / 4) reserve(capacity_ / 2);
        }
        void erase(int index){
            if(index < 0 || index >= size_){
                throw std::out_of_range("Iterator out of range");
            }
            for(int i = index; i < size_ - 1; i++) data[i] = data[i + 1];
            size_--;
            if (size_ > 0 && size_ <= capacity_ / 4) reserve(capacity_ / 2);
        }
        
        void clear(){
            size_ = 0;
        }
        void reserve(int new_capacity_){
            if(new_capacity_ <= capacity_) return;
            T* new_data = new T[new_capacity_];
            for(int i = 0; i < size_; i++) new_data[i] = data[i];
            delete[] data;
            data = new_data;
            capacity_ = new_capacity_;
        }
        void resize_(int new_size_){
            if(new_size_ >capacity_){
                reserve(new_size_);
            }
            for(int i = size_; i < new_size_; i++) data[i] = T();
            size_ = new_size_;
        }
        void fit_capacity_(){
            if(capacity_ <= size_) return;
            if(size_ == 0){ 
                delete[] data;
                data = nullptr;
                capacity_ = 0;
                return;
            }
            T* new_data = new T[size_];
            for(int i = 0; i < size_;  i++) new_data[i] = data[i];
            delete[] data;
            data = new_data;
            capacity_ = size_;
            
        }        
};