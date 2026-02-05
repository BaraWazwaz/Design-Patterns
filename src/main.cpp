#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "table.hpp"

int main()
{
    nitron::Table table;
    table.addField<int>("id", 0);
    table.addRow();
    table.getRow(0).set(0, 100);
    std::cout << table;
    return 0;
}