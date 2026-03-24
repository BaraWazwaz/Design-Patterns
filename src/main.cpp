#include <iostream>
#include <sstream>
#include "datastructures/StaticArray.hpp"
#include "tests/Spec.hpp"

int main()
{
    using nitron::Spec;
    using nitron::Test;
    using Arr = nitron::StaticArray<int,10>;

    Spec main ("Main");
    main
        .openSubSpec("StaticArray")
            .addTest(Test::checkReturnValue<Arr>(
                []() {
                    return Arr({1,2,3,4,5});
                },
                [](Arr a){
                    return a.at(0) == 1 && a.at(4) == 5;
                },
                "Constructor and .at()"))
            .addTest(Test::checkReturnValue<Arr>(
                []() -> Arr {
                    return Arr({1, 2, 3, 4, 5, 6, 7})
                           .transform<int>([](int x) { return x * x; });
                },
                [](Arr x) {
                    return (x <=> Arr({1, 4, 9, 16, 25, 36, 49})) == 0;
                },
                "Spaceship operator (operator<=>)"
            ))
            .addTest(Test::checkThrowType<std::out_of_range>(
                []() {
                    Arr a = {1,2,3,4,5};
                    return a.at(10);
                },
                "Illegal .at() should throw std::out_of_range"))
        .closeSubSpec()
        .addTest(Test::checkThrowValue<int>(
            []() {
                throw 10;
            },
            [](int x) {
                return x == 0;
            },
            "This should fail"));
    main.state(std::cout);

    return 0;
}