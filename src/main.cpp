#include <iostream>
#include <sstream>
#include "datastructures/StaticArray.hpp"
#include "datastructures/Iterator.hpp"
#include "tests/Spec.hpp"

void runTests()
{
    using nitron::Spec;
    using nitron::Test;
    using Itr = nitron::Iterator<int>;
    using Arr = nitron::StaticArray<int,10>;
    using Ars = nitron::StaticArray<std::string,10>;
    using Its = nitron::Iterator<std::string>;
    
    Ars a = {"Ahmad","Bara","Carl","Dann"};
    const Ars& ac = a;
    
    Spec main ("Main");
    
    main
    
    .openSubSpec("Iterator")
        .addTest(Test::checkReturnValue<Its>(
            [&a]() { return a.begin(); },
            [&ac](Its it) { return typeid(it) != typeid(ac.begin()); },
            "begin() differs from begin() const"
        ))
    .closeSubSpec()
    
    .openSubSpec("StaticArray")
        .addTest(Test::checkReturnValue<Arr>(
            [](){ return Arr({1,2,3,4,5}); },
            [](Arr a){ return a.at(0) == 1 && a.at(4) == 5; },
            "Constructor and .at()"
        ))
        .addTest(Test::checkReturnValue<Arr>(
            [](){ return Arr({1, 2, 3, 4, 5, 6, 7}).transform<int>([](int x) { return x * x; }); },
            [](Arr x) { return (x <=> Arr({1, 4, 9, 16, 25, 36, 49})) == 0; },
            "Spaceship operator (operator<=>)"
        ))
        .addTest(Test::checkThrowType<std::out_of_range>(
            [](){ Arr a = {1,2,3,4,5}; return a.at(10); },
            "Illegal .at() should throw std::out_of_range"
        ))
    .closeSubSpec()
    
    .openSubSpec("Specs")
        .addTest(Test::checkReturnValue<int>(
            [](){ return 10; },
            [](int x) { return x == 0; },
            "return value fail"
        ).toFail())
        .addTest(Test::checkReturnValue<int>(
            [](){ return 10; },
            [](int x) { return x == 10; },
            "return value pass"
        ))
        .addTest(Test::checkThrowValue<int>(
            [](){ throw 10; },
            [](int x) { return x == 0; },
            "throw value fail"
        ).toFail())
        .addTest(Test::checkThrowValue<int>(
            [](){ throw 10; },
            [](int x) { return x == 10; },
            "throw value pass"
        ))
        .addTest(Test::checkThrowType<int>(
            [](){ throw 10ll; },
            "throw type fail"
        ).toFail())
        .addTest(Test::checkThrowType<int>(
            [](){ throw 10; },
            "throw type pass"
        ))
    .closeSubSpec();
    
    main.state(std::cout);
}

int main()
{
    runTests();
    return 0;
}