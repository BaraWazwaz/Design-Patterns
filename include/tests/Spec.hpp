#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

namespace nitron
{

class Test
{
public:

    template <typename T>
    static Test checkThrowType(const std::function<void()>& function,
                               const std::string& message = "[No Message]");

    template <typename T>
    static Test checkThrowValue(const std::function<void()>& function,
                                std::function<bool(T)> checker,
                                const std::string& message = "[No Message]");

    template <typename T>
    static Test checkReturnValue(const std::function<T()>& function,
                                 std::function<bool(T)> checker,
                                 const std::string& message = "[No Message]");

    bool state(std::ostream& os, std::size_t tabs = 0);

private:
    bool verdict;
    std::string message;

    Test() = delete;
    Test(bool verdict, const std::string& message);
};

class Spec
{
public:

    Spec() = default;
    Spec(const std::string& title);

    Spec& closeSubSpec();
    Spec& openSubSpec(const std::string& title);
    Spec& addTest(Test&& test);

    bool state(std::ostream& os, std::size_t tabs = 0);

private:

    std::string title = "Untitled";
    Spec* parent = this;
    std::vector<Test> direct;
    std::vector<Spec> children;
};

} // namespace nitron

#include "Spec.tpp"