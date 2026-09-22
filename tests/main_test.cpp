#include<iostream>
#include "test_framework.hpp"
#include"dsa/priorityqueue.h"
int main() {
    Priorityqueue<int> a,b,c;
    for(int i = 0; i <= 10; i++) a.push(i);
    for(int i = 0; i <= 10; i++) std::cout<<a.top();
    return epfd::test::TestRegistry::instance().runAll();
}
