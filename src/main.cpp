#include <iostream>
#include <sstream>
#include <Table.hpp>
#include "tests/Spec.hpp"

void runTests()
{
    using nitron::Spec;
    using nitron::Test;
    using nitron::Record;
    using nitron::Table;

    Spec main ("Main");
    
    main
    .openSubSpec("nitron::Table")
        .addTest(Test::checkReturnValue<std::string>(
            []() -> std::string {
                Table table;

                table.emplaceBackField<int>("id", 0);
                table.emplaceBackField<std::string>("name", "Anonymous");
                table.emplaceBackField<bool>("active", true);

                Record record;
                record.emplaceBackField<int>(1);
                record.emplaceBackField<std::string>(std::string("Ahmad"));
                record.emplaceBackField<bool>(true);
                table.emplaceBackRecord(std::move(record));

                std::stringstream str;
                str << table;
                return str.str();
            },
            [](std::string const& tableConent) -> bool {
                return tableConent ==
                "----------------------------------------------\n"
                "|           id |         name |       active |\n"
                "----------------------------------------------\n"
                "|            1 |        Ahmad |            1 |\n"
                "----------------------------------------------\n";
            },
            "nitron::Table can recieve records and output it correctly."
        ))
    .closeSubSpec();
    
    main.state(std::cout);
}

int main()
{
    runTests();
    return 0;
}