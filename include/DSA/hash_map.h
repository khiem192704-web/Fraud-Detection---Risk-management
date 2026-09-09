#pragma once

#include<iostream>
#include<stdexcept>
#include<string>
#include"Vector.h"
template<typename K, typename V>
struct Node{
   V value;
   K key;
   Node* next;
   Node(const K& k, const V& v) : value(v), key(k), next(nullptr){}
};
template<typename K, typename V>
class Hash_map
{
    private:
         Node<K,V>** table;
         int size;
         int function(const K& key) const {
            unsigned long long 
         }

         
};