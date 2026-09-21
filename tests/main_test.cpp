#include<iostream>
#include "test_framework.hpp"
#include"include/DSA/Priorityqueue.h"
int main() {
    Priorityqueue<int> a,b,c;
    for(int i = 0; i <= 10; i++) a.push(i);
    for(int i = 0; i <= 10; i++) std::cout<<a.pop();
    return epfd::test::TestRegistry::instance().runAll();
}
