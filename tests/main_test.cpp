#include <iostream>
#include "dsa/vector.h"

int main() {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    std::cout << "Kich thuoc: " << vec.size() << std::endl;
    for (int i = 0; i < vec.size(); i++) {
        std::cout << "vec[" << i << "] = " << vec[i] << std::endl;
    }

    return 0;
}