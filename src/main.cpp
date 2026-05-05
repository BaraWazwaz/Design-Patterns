#include <iostream>
#include <sstream>
#include "Table.hpp"
#include "Promise.hpp"
#include "tests/Spec.hpp"

void runTests()
{
    using nitron::Spec;
    using nitron::Test;
    using nitron::Record;
    using nitron::Table;
    using Promise = nitron::Promise<int>;

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
    .closeSubSpec()
    .openSubSpec("nitron::Promise")
        .addTest(Test::checkReturnValue<int>(
            []() {
                Promise p (std::plus<int>(), 4, 7);
                return p.get();
            },
            [](long long const& x) { return x == 11; },
            "nitron::Promise<long long> should resolve on get()"
        ))
        .addTest(Test::checkThrowType<std::runtime_error>(
            []() {
                Promise p ([](){
                    throw std::runtime_error("Hello");
                    return 0;
                });
                p.get();
            },
            "nitron::Promise<long long> should receive a thrown std::runtime_error on get()"
        ))
    .closeSubSpec();
    
    main.state(std::cout);
}

int experimenting()
{
    return 0;
}

int main()
{
    int status = experimenting();
    if (status != 0)
        return status;
    else
        runTests();
    return 0;
}