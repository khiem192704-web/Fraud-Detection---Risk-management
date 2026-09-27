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
        Vector(int initSize) : size_(initSize), capacity_(initSize){
            this->data = new T[this->capacity];
            for(int i = 0; i < this->size_; i++){
                this->data[i] = T();
            }
        }
        Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_){
            if(other.data == nullptr){
                this->data = nullptr;
                return;
            }
            this->data = new T[this->capacity_];
            for(int i = 0; i < size_; i++){
                this->data[i] = other.data[i];
            }
        }
        ~Vector(){ if(this->data != nullptr) delete[] this->data; }
        //(a = b) = c
        const Vector& operator=(const Vector& other) {
            if(this != &other){
                delete[] this->data;
                this->data = new T[other.capacity_];
                this->size_ = other.size_;
                this->capacity_ = other.capacity_;
                for(int i = 0; i < size_; i++){
                    this->data[i] = other.data[i];
                }
            }
            return *this;
        }
        T& operator[](int index) {
            return this->data[index];
        }
        const T& operator[](int index) const{
            return this->data[index];
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
            return this->size_ == 0;
        }

        T& front(){
            if (empty()) {
                throw std::out_of_range("Vector is empty");
            }
            return this->data[0];
        }

        const T& front() const{
            if (empty()) {
                throw std::out_of_range("Vector is empty");
            }
            return this->data[0];
        }

        T& back() {
            if (empty()) {
                throw std::out_of_range("Vector is empty");
            }
            return this->data[this->size_ - 1];
        }

        const T& back() const{
            if (empty()) {
                throw std::out_of_range("Vector is empty");
            }
            return this->data[this->size_ - 1];
        }

        int size() const{
            return this->size_;
        }
        int capacity() const{
            return this->capacity_;
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
            if(this->size_ == this->capacity_){
                grow();
            }
            this->data[size_++] = value;
        }

        void insert(int index, const T& value){
            if(index < 0 || index > this->size_){
                throw std::out_of_range("Insert index out of range");
            }
            if(this->size_ == this->capacity_) grow();
            for(int i = this->size_; i > index; i--) this->data[i] = this->data[i - 1];
            this->data[index] = value;
            this->size_++;
        }

        void pop_back(){
            if(empty()){
                throw std::out_of_range("Vector is empty");
            }
            this->size_--;
            if (this->size_ > 0 && 2 * this->size_ <= this->capacity_ / 4) fit_capacity();
        }
        void erase(int index){
            if(index < 0 || index >= size_){
                throw std::out_of_range("Iterator out of range");
            }
            for(int i = index; i < this->size_ - 1; i++) this->data[i] = this->data[i + 1];
            this->size_--;
            if (this->size_ > 0 && 2 * this->size_ <= this->capacity_ / 4) fit_capacity();
        }

        void clear(){
            this->size_ = 0;
        }
        void reserve(int new_capacity_){
            if(new_capacity_ <= this->capacity_) return;
            T* new_data = new T[new_capacity_];
            for(int i = 0; i < size_; i++) new_data[i] = this->data[i];
            delete[] this->data;
            this->data = new_data;
            this->capacity_ = new_capacity_;
        }
        void resize(int new_size_){
            if(new_size_ > this->capacity_){
                reserve(new_size_);
            }
            for(int i = this->size_; i < new_size_; i++) this->data[i] = T();
            this->size_ = new_size_;
        }
        void fit_capacity(){
            if(this->capacity_ <= this->size_) return;
            if(this->size_ == 0){
                delete[] this->data;
                this->data = nullptr;
                this->capacity_ = 0;
                return;
            }
            T* new_data = new T[this->size_];
            for(int i = 0; i < this->size_;  i++) new_data[i] = this->data[i];
            delete[] this->data;
            this->data = new_data;
            this->capacity_ = this->size_;

        }
};
