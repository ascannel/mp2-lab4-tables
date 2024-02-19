#include "table.hpp"
#include <iostream>

int main() {
    SimpleTable<int, int> table;
    table.insert(1, 2);
    table.insert(2, 3);
    auto res = table.find(1);
    std::cout << *res << endl;
    std::cout << table.getSize() << endl;
    vector<int> t;
}