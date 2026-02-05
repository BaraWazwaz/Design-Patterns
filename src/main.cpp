#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "table.hpp"

int main()
{
    nitron::Table table;
    table.addField<int>("Id", -1);
    table.addField<std::string>("Name", std::string("N/A"));
    table.addField<double>("Score", 0.0);
    table.addField<bool>("Active", false);

    for (int i = 0; i < 5; ++i) {
        table.addRow();
        table.getRow(i).set(0, 100 + i);
        table.getRow(i).set(1, "Entity_" + std::to_string(i));
        table.getRow(i).set(2, 75.5 + (i * 2.25));
        table.getRow(i).set(3, (i % 2 == 0));
    }

    table.getRow(2).set(1, std::string("Updated_Entity"));
    table.getRow(2).set(2, 99.99);
    table.getRow(4).set(0, 999);

    std::cout << table << std::endl;
    return 0;
}