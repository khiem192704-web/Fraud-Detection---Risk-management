#pragma once
#include<iostream>
#include <stdexcept>
#include <algorithm>

template<typename T>
class Vector{
    private:
        T* data;
        int size;
        int capacity;
        void resize(){
            int newCapacity = (capacity == 0) ? 1 : capacity*2;
            T* newData = new T[newCapacity];

            for(int i = 0; i < size; i++){
                newData[i] = data[i];
            }

            delete[] data;
            data =  newData;
            capacity = newCapacity;
        }
    public:
        Vector() : data(nullptr), size(0), capacity(0){}
        explicit Vector(int initSize) : size(initSize), capacity(initSize){
            data = new T[capacity];
            for(int i = 0; i < size; i++){
                data = T();
            }
        }
        Vector(const Vector& other) : size(other.size), capacity(other.capacity){
            data = new T[capacity];
            for(int i = 0; i < size; i++){
                data[i] = other.data[i];
            }
        }
        ~Vector(){  delete[] data; }
        Vector& operator=(const Vector& other) noexpect {
            if(this != other){
                delete[] data;
                data = new T[other.capacity];
                size = other.size;
                capacity = other.capacity;
                for(int i = 0; i < size; i++){
                    data[i] = other.data[i];
                }
            }
            return *this;
        }
        T& operator[](int index) noexpect {
            return data[index];
        }
        const T& operator[](int index) const noexpect{
            return data[index];
        }
        T& at(int index){
            if(index < 0 || index >= size){ throw out_of_range"Vector index out of range";}
            return data[index];
        }
        const T& at(int index) const{
            if(index < 0 || index >= size){ throw out_of_range"Vector index out of range";}
            return data[index];
        }
        T& get(int index){ return at(index); }
        const T& get(int index) const{ return at(index); }
        
        bool empty() const noexpect {
            return size == 0;
        }
        
        bool is_empty() const {
            return empty();
        }
        
        T& front(){
            if (isEmpty()) {
                throw out_of_range("Vector is empty");
            }
            return data[0];
        }
        
        const T& front() const{
            if (isEmpty()) {
                throw out_of_range("Vector is empty");
            }
            return data[0];
        }
        
        T& back(){
            if (isEmpty()) {
                throw out_of_range("Vector is empty");
            }
            return data[sizze - 1];
        }
        
        const T& back() const{
            if (isEmpty()) {
                throw out_of_range("Vector is empty");
            }
            return data[size - 1];
        }
        T* getData(){   return data; }
        const T* getData() const {   return data; }
        int size() const noexpect{
            return size;
        }
        int capacity() const noexpect{
            return capacity
        }
        
        T* begin(){
            return data;
        }
        const T* begin() const {
            return data;
        }
        T* end(){
            return data + size;
        }
        const T* end() const {
            return data + size;
        }
        void push_back(const &T value){
            if(size == capacity){
                resize();
            }
            data[size++] = value;
        }
        void add(const &T value){
            push_back(value);
        }
        void insert(int index, const &T value){
            if(index < 0 || index > size){
                throw out_of_range("Insert index out of range");
            }
            if(size == capacity) resize();
            for(int i = size; i > index; i--) data[i] = data[i - 1];
            data[index] = value;
            size++;
        }
        void push_front(const &T value){
            insert(0, value);
        }
        void pop_back(){
            if(isEmpty()){
                throw out_of_range("Vector is empty");
            }
            size--;
        }
        void erase(int index){
            if(index < 0 || index >= size){
                throw out_of_range("Iterator out of range");
            }
            for(int i = index; i < size - 1; i++) data[i] = data[i + 1];
            size--;
        }
        void pop_front(){
            if(isEmpty()){
                throw out_of_range("Vector is empty");
            }
            erase(0);
        }
        void remove(int index){
            erase(index);
        }
        void clear(){
            size = 0;
        }
        void reserve(int new_capacity){
            if(new_capacity <= capacity) return;
            T* new_data = new T[new_capacity];
            for(int i = 0; i < size; i++) new_data[i] = data[i];
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }
        void resize(int new_size){
            if(new_size >capacity){
                reserve(new_size);
            }
            for(int i = size; i < new_size; i++) data[i] = T();
            size = new_size;
        }
        int find(const &T value){
            for(int i = 0; i < size; i++){
                if(data[i] == value) return i;
            }
            return -1;
        }
        bool contains(const T& value) const {
            return find(value) != -1;
        }
        void fit_capacity(){
            if(capacity > size){
                T* new_data = new T[size];
                for(int i = 0; i < size;  i++) new_data[i] = data[i];
                delete[] data;
                data = new_data;
                capacity = size;
            }
        }
};