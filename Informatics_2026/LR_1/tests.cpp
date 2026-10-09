#include <iostream>
#include "myvector.h"
#include "mydeque.h"

int main() {
    MyDeque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_front(0);
    d.push_front(-1);

    // ожидаем: [-1, 0, 1, 2]
    for (size_t i = 0; i < d.size(); ++i) {
        std::cout << d[i] << " ";
    }
    std::cout << "\n";

    d.pop_front();   // ожидаем: [0, 1, 2]
    d.pop_back();    // ожидаем: [0, 1]
    for (size_t i = 0; i < d.size(); ++i) {
        std::cout << d[i] << " ";
    }
    std::cout << "\n";

    std::cout << "front=" << d.front() << " back=" << d.back() << "\n";
    return 0;
}