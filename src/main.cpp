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

    Table table;

    table.emplaceBackField<int>("id", 0);
    table.emplaceBackField<std::string>("name", "Anonymous");
    table.emplaceBackField<bool>("active", true);

    Record record;
    record.emplaceBackField<int>(1);
    record.emplaceBackField<std::string>(std::string("Ahmad"));
    record.emplaceBackField<bool>(true);
    table.emplaceBackRecord(std::move(record));

    std::cout << table << std::endl;

    Spec main ("Main");
    
    // main
    
    // .openSubSpec("Iterator")
    //     .addTest(Test::checkReturnValue<Its>(
    //         [&a](){ return a.begin(); },
    //         [&ac](Its it){ return typeid(it) != typeid(ac.begin()); },
    //         "begin() differs from begin() const"
    //     ))
    //     .addTest(Test::checkThrowType<std::runtime_error>(
    //         [](){ *Itr(nullptr); },
    //         "Dereferecing null pointer throws std::runtime_error"
    //     ))
    //     .addTest(Test::checkThrowType<std::runtime_error>(
    //         [](){ Its(nullptr)->size(); },
    //         "Accessing a method of a null pointer throws std::runtime_error"
    //     ))
    // .closeSubSpec()
    
    // .openSubSpec("StaticArray")
    //     .addTest(Test::checkReturnValue<Arr>(
    //         [](){ return Arr({1,2,3,4,5}); },
    //         [](Arr a){ return a.at(0) == 1 && a.at(4) == 5; },
    //         "Constructor and .at()"
    //     ))
    //     .addTest(Test::checkReturnValue<Arr>(
    //         [](){ return Arr({1, 2, 3, 4, 5, 6, 7}).transform<int>([](int x) { return x * x; }); },
    //         [](Arr x){ return (x <=> Arr({1, 4, 9, 16, 25, 36, 49})) == 0; },
    //         "Spaceship operator (operator<=>)"
    //     ))
    //     .addTest(Test::checkThrowType<std::out_of_range>(
    //         [](){ Arr a = {1,2,3,4,5}; return a.at(10); },
    //         "Illegal .at() should throw std::out_of_range"
    //     ))
    //     .addTest(Test::checkReturnValue<int>(
    //         [&a](){ int x = 0; a.forEach([&x](const std::string& s){ x += s.size(); }); return x; },
    //         [](int x){ return x == 17; },
    //         "forEach()"
    //     ))
    //     .addTest(Test::checkReturnValue<std::string>(
    //         [&a](){ return a.reduce<std::string>(std::plus<std::string>(), ""); },
    //         [](const std::string& s) { return s == "AhmadBaraCarlDann"; },
    //         "reduce()"
    //     ))
    //     .addTest(Test::checkReturnValue<Arr>(
    //         [&a](){ return a.transform<int>([](const std::string& s){ return s.size(); }); },
    //         [](Arr a){ return (a <=> Arr({5,4,4,4})) == 0; },
    //         "transform()"
    //     ))
    // .closeSubSpec()
    
    // .openSubSpec("Specs")
    //     .addTest(Test::checkReturnValue<int>(
    //         [](){ return 10; },
    //         [](int x) { return x == 0; },
    //         "return value fail"
    //     ).toFail())
    //     .addTest(Test::checkReturnValue<int>(
    //         [](){ return 10; },
    //         [](int x) { return x == 10; },
    //         "return value pass"
    //     ))
    //     .addTest(Test::checkThrowValue<int>(
    //         [](){ throw 10; },
    //         [](int x) { return x == 0; },
    //         "throw value fail"
    //     ).toFail())
    //     .addTest(Test::checkThrowValue<int>(
    //         [](){ throw 10; },
    //         [](int x) { return x == 10; },
    //         "throw value pass"
    //     ))
    //     .addTest(Test::checkThrowType<int>(
    //         [](){ throw 10ll; },
    //         "throw type fail"
    //     ).toFail())
    //     .addTest(Test::checkThrowType<int>(
    //         [](){ throw 10; },
    //         "throw type pass"
    //     ))
    // .closeSubSpec();
    
    // main.state(std::cout);
}

int main()
{
    runTests();
    return 0;
}