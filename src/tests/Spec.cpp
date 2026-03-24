#include "tests/Spec.hpp"

namespace nitron
{

bool Test::state(std::ostream& os, std::size_t tabs)
{
    if (verdict)
        os << std::string(tabs, '\t') << "[Passed]: { " << message << " }\n";
        else
        os << std::string(tabs, '\t') << "[Passed]: { " << message << " }\n";
    return verdict;
}

Test::Test(bool verdict, const std::string& message) :
verdict(verdict),
    message(message)
{}

Spec::Spec(const std::string& title) :
title(title)
{}

Spec& Spec::openSubSpec(const std::string& title)
{
    children.emplace_back(title);
    children.back().parent = this;
    return children.back();
}

Spec& Spec::closeSubSpec()
{
    return *parent;
}

Spec& Spec::addTest(Test&& test)
{
    direct.emplace_back(test);
    return *this;
}

bool Spec::state(std::ostream& os, std::size_t tabs)
{
    bool verdict = true;
    os << std::string(tabs, '\t') << "Spec { " << title << " } :\n";
    for (auto& test : direct)
    {
        verdict = test.state(os, tabs + 1) && verdict;
    }
    for (auto& spec : children)
    {
        verdict = spec.state(os, tabs + 1) && verdict;
    }
    if (verdict)
    {
        os << std::string(tabs + 1, '\t') << "Verdict: [Passed]\n";
    }
    else
    {
        os << std::string(tabs + 1, '\t') << "Verdict: [Failed]\n";
    }
    return verdict;
}

} // namespace nitron
