#pragma once
#include <stdexcept>

template<typename T>
class Deque{
    private:
        T* data;
        int capacity;
        int frontIndex;
        int count;

        void resize(int newCapacity){
            T* newData = new T[newCapacity];
            for(int i = 0; i < count; i++){
                newData[i] = data[(frontIndex + i) % capacity];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
            frontIndex = 0;
        }

    public:
        Deque() : capacity(8), frontIndex(0), count(0){
            data = new T[capacity];
        }

        Deque(const Deque& other) : capacity(other.capacity), frontIndex(0), count(other.count){
            data = new T[capacity];
            for(int i = 0; i < count; i++){
                data[i] = other.data[(other.frontIndex + i) % other.capacity];
            }
        }

        Deque& operator=(const Deque& other){
            if(this != &other){
                capacity = other.capacity;
                count = other.count;
                frontIndex = 0;
                data = new T[capacity];
                for(int i = 0; i < count; i++){
                    data[i] = other.data[(other.frontIndex + i)%other.capacity];
                }
            }
            return *this;
        }

        ~Deque(){
            delete[] data;
        }

        bool empty() const noexcept{
            return count == 0;
        }

        int size() const noexcept{
            return count;
        }

        void push_back(const T& value){
            if(count == capacity){
                resize(capacity * 2);
            }
            int index = (frontIndex + count) % capacity;
            data[index] = value;
            count++;
        }

        void push_front(const T& value){
            if(count == capacity){
                resize(capacity * 2);
            }
            frontIndex = (frontIndex - 1 + capacity) % capacity;
            data[frontIndex] = value;
            count++;
        }

        void pop_back(){
            if(empty()) return;
            count--;
        }

        void pop_front(){
            if(empty()) return;
            frontIndex = (frontIndex + 1) % capacity;
            count--;
        }

        T& front(){
            if(empty()){
                throw std::out_of_range("Deque is empty!");
            }
            return data[frontIndex];
        }

        const T& front() const{
            if(empty()){
                throw std::out_of_range("Deque is empty!");
            }
            return data[frontIndex];
        }

        T& back(){
            if(empty()){
                throw std::out_of_range("Deque is empty!");
            }
            int index = (frontIndex + count - 1) % capacity;
            return data[index];
        }

        const T& back() const{
            if(empty()){
                throw std::out_of_range("Deque is empty!");
            }
            int index = (frontIndex + count - 1) % capacity;
            return data[index];
        }

        T& operator[](int index){
            if(index < 0 || index >= count){
                throw std::out_of_range("Deque index out of range!");
            }
            return data[(frontIndex + index) % capacity];
        }

        const T& operator[](int index) const{
            if(index < 0 || index >= count){
                throw std::out_of_range("Deque index out of range!");
            }
            return data[(frontIndex + index) % capacity];
        }

        void clear() noexcept{
            count = 0;
            frontIndex = 0;
        }
};