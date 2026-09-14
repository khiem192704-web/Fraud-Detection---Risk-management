#include<iostream>
#include "test_framework.hpp"
#include"include/DSA/Vector.h"
int main() {
    Vector<int> a,b,c;
    for(int i = 0; i <= 10; i++) a.push_back(i);
    for(int i = 0; i <= 10; i++) std::cout<<a.pop_back();
    return epfd::test::TestRegistry::instance().runAll();
}
