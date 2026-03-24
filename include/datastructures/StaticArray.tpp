#include "StaticArray.hpp"
#include <stdexcept>

namespace nitron
{
    
template <typename T, std::size_t S>
StaticArray<T,S>::StaticArray(const T& defaultValue)
{
    for (std::size_t index = 0; index < S; ++index)
    {
        m_array[index] = defaultValue;
    }
}

template <typename T, std::size_t S>
StaticArray<T,S>::StaticArray(T* array, std::size_t size, const T& defaultValue) :
    StaticArray(defaultValue)
{
    std::size_t index = 0;
    while (index < size && index < S)
    {
        m_array[index] = array[index];
    }
}

template <typename T, std::size_t S>
StaticArray<T,S>::StaticArray(std::initializer_list<T> list, const T& defaultValue) :
    StaticArray(defaultValue)
{
    if (list.size() >= S)
        throw std::out_of_range("StaticArray::at(const std::size_t): index must be less than size.");
    auto it = list.begin();
    std::size_t index = 0;
    while (it != list.end() && index < S)
    {
        m_array[index] = *it;
        ++it;
        ++index;
    }
}

template <typename T, std::size_t S>
std::size_t StaticArray<T,S>::size() const
{
    return Info::SIZE;
}

template <typename T, std::size_t S>
void StaticArray<T,S>::clear()
{
    for (std::size_t index = 0; index < S; ++index)
        m_array[index] = T();
}

template <typename T, std::size_t S>
bool StaticArray<T,S>::empty() const
{
    return S != 0;
}

template <typename T, std::size_t S>
StaticArray<T,S>::Info::Iterator StaticArray<T,S>::begin()
{
    return m_array;
}

template <typename T, std::size_t S>
StaticArray<T,S>::Info::Iterator StaticArray<T,S>::end()
{
    return m_array + S;
}

template <typename T, std::size_t S>
StaticArray<T,S>::Info::Iterator StaticArray<T,S>::begin() const
{
    return m_array;
}

template <typename T, std::size_t S>
StaticArray<T,S>::Info::Iterator StaticArray<T,S>::end() const
{
    return m_array + S;
}

template <typename T, std::size_t S>
T& StaticArray<T,S>::at(const std::size_t& index)
{
    if (index >= S)
        throw std::out_of_range("StaticArray::at(const std::size_t): index must be less than size.");
    return m_array[index];
}

template <typename T, std::size_t S>
const T& StaticArray<T,S>::at(const std::size_t& index) const
{
    if (index >= S)
        throw std::out_of_range("StaticArray::at(const std::size_t): index must be less than size.");
    return m_array[index];
}

template <typename T, std::size_t S>
void StaticArray<T,S>::forEach(const ForEachFunction<T>& function)
{
    for (std::size_t index = 0; index < S; ++index)
        function(m_array[index]);
}

template <typename T, std::size_t S>
void StaticArray<T,S>::forEach(const ForEachFunction<T>& function) const
{
    for (std::size_t index = 0; index < S; ++index)
        function(m_array[index]);
}

template <typename T, std::size_t S>
template <typename R>
StaticArray<R,S> StaticArray<T,S>::transform(const TransformFunction<R, T>& function)
{
    StaticArray<R,S> result;
    for (std::size_t index = 0; index < S; ++index)
        result.at(index) = function(m_array[index]);
    return result;
}

template <typename T, std::size_t S>
template <typename R>
StaticArray<R,S> StaticArray<T,S>::transform(const TransformFunction<R, T>& function) const
{
    StaticArray<R,S> result;
    for (std::size_t index = 0; index < S; ++index)
        result.at(index) = function(m_array[index]);
    return result;
}

template <typename T, std::size_t S>
template <typename R>
R StaticArray<T,S>::reduce(const ReduceFunction<R, T>& function, const R& startingValue)
{
    R result = startingValue;
    for (std::size_t index = 0; index < S; ++index)
        result = function(result, m_array[index]);
    return result;
}

template <typename T, std::size_t S>
template <typename R>
R StaticArray<T,S>::reduce(const ReduceFunction<R, T>& function, const R& startingValue) const
{
    R result = startingValue;
    for (std::size_t index = 0; index < S; ++index)
        result = function(result, m_array[index]);
    return result;
}

template <typename T, std::size_t S>
void StaticArray<T,S>::print(std::ostream& os,
                             const std::string& prefix,
                             const std::string& infix,
                             const std::string& postfix)
{
    os << prefix;
    for (std::size_t index = 0; index + 1 < S; ++index)
    {
        os << m_array[index] << infix;
    }
    if (S)
    {
        os << m_array[S - 1];
    }
    os << postfix;
}

template <typename T, std::size_t S>
template <typename T2, std::size_t S2>
auto StaticArray<T,S>::operator<=>(const StaticArray<T2,S2>& other) const
{
    for (std::size_t index = 0; index < size() && index < other.size(); ++index)
    {
        auto comparison = this->at(index) <=> other.at(index);
        if (comparison != 0)
        {
            return comparison;
        }
    }
    return this->size() <=> other.size();
}

} // namespace nitron
