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

        void resize(int newCapacity){//Cấp phát vùng nhớ mới.
            T* newData = new T[newCapacity];
            for(int i = 0; i < count; i++){//Sao chép phần tử
                newData[i] = data[(frontIndex + i) % capacity];
            }
            delete[] data; //Giải phóng bộ nhớ cũ
            data = newData; //Cập nhật con trỏ
            capacity = newCapacity; //Cập nhật dung lượng
            frontIndex = 0; //Đặt lại frontIndex
        }

    public:
        //default constructor
        Deque() : capacity(8), frontIndex(0), count(0){
            data = new T[capacity];
        }
        //copy constructor Nó được gọi khi tạo object mới từ object cũ
        Deque(const Deque& other) : capacity(other.capacity), frontIndex(0), count(other.count){
            data = new T[capacity];
            for(int i = 0; i < count; i++){
                data[i] = other.data[(other.frontIndex + i) % other.capacity];
            }
        }
        //copy assignment operator Nó được gọi khi gán giá trị của object này cho object khác
        Deque& operator=(const Deque& other){
            if(this != &other){
                T* newData = new T[other.capacity];
                for(int i = 0; i < other.count; i++){
                    newData[i] = other.data[(other.frontIndex + i)%other.capacity];
                }
                delete[] data;
                data=newData;
                capacity = other.capacity;
                count = other.count;
                frontIndex = 0;
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