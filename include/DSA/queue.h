#pragma once
#include<iostream>
#include"Deque.h"

template<typename T>

class Queue
{
    private:
        Deque<T> data;
    public:
        Queue():data(){}

        bool empty() const noexcept{
            return data.empty();
        }

        void clear() const noexcept{
            return data.clear();
        }

        int size() const noexcept{
            return data.size();
        }

        void push(const T& value){
            data.push_back(value);
        }

        void pop(){
            data.pop_front();
        }

        T& front(){
            return data.front();
        }

        const T& front() const{
            return data.front();
        }

        T& back(){
            return data.back();
        }

        const T& back() const {
            return data.back();
        }
};