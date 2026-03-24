#include "tests/Spec.hpp"

namespace nitron
{

bool Test::state(std::ostream& os, std::size_t tabs)
{
    if (verdict)
        os << std::string(tabs, '\t') << "[Passed]: { " << message << " }\n";
    else
        os << std::string(tabs, '\t') << "[Failed]: { " << message << " }\n";
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
    
    os << std::string(tabs + 1, '\t') << "-------------------\n";
    for (auto& spec : children)
    {
        verdict = spec.state(os, tabs + 1) && verdict;
        os << std::string(tabs + 1, '\t') << "-------------------\n";
    }

    if (verdict)
    {
        os << std::string(tabs + 1, '\t') << "Verdict: [Passed]\n";
    }
    else
    {
        os << std::string(tabs + 1, '\t') << "Verdict: [Failed]\n";
    }
    os << std::string(tabs + 1, '\t') << "===================\n";

    return verdict;
}

Test Test::toPass() &
{
    Test copy = *this;
    return copy;
}

Test Test::toPass() &&
{
    return Test(verdict, std::move(message));
}

Test Test::toFail() &
{
    Test copy = *this;
    copy.verdict = !copy.verdict;
    return copy;
}

Test Test::toFail() &&
{
    return Test(!verdict, std::move(message));
}



} // namespace nitron
