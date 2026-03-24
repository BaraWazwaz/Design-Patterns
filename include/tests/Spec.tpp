#include "Spec.hpp"

namespace nitron
{

template <typename T>
Test Test::checkThrowType(const std::function<void()>& function,
                          const std::string& message)
{
    bool verdict = false;
    try { function(); }
    catch(const T& x) { verdict = false; }
    catch(...) {}
    return Test(verdict, message);
}

template <typename T>
Test Test::checkThrowValue(const std::function<void()>& function,
                           std::function<bool(T)> checker,
                           const std::string& message)
{
    bool verdict = false;
    try { function(); }
    catch(const T& x) { verdict = checker(x); }
    catch(...) {}
    return Test(verdict, message);
}

template <typename T>
Test Test::checkReturnValue(const std::function<T()>& function,
                             std::function<bool(T)> checker,
                             const std::string& message)
{
    bool verdict = false;
    try { verdict = checker(function()); }
    catch(...) {}
    return Test(verdict, message);
}

} // namespace nitron
