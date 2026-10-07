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

        bool empty() const {
            return data.empty();
        }

        void clear() const {
            return data.clear();
        }

        int size() const {
            return data.size();
        }

        void EnQueue(const T& value){
            data.push_back(value);
        }

        void DeQueue(){
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