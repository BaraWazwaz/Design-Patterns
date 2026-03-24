#pragma once

#include <iosfwd>
#include "DataStructures.hpp"
#include "Iterator.hpp"

namespace nitron
{

template <typename T, std::size_t S>
class StaticArray
{
public:
    struct Info : public LinearDSTag
    {
        using KeyType = std::size_t;
        using ValueType = T;
        using Iterator = nitron::Iterator<T>;
        using IteratorConst = nitron::Iterator<const T>;
        inline static std::size_t const SIZE = S;

        template <typename T_, std::size_t S_>
        using StaticArray = StaticArray<T_,S_>;
    };

    std::size_t size() const;
    void clear();
    bool empty() const;

    Info::Iterator begin();
    Info::Iterator end();
    Info::IteratorConst begin() const;
    Info::IteratorConst end() const;
    
    T& at(const std::size_t& index);
    const T& at(const std::size_t& index) const;
    
    void forEach(const ForEachFunction<T>& function);
    void forEach(const ForEachFunction<T>& function) const;
    
    template <typename R>
    StaticArray<R, S> transform(const TransformFunction<R, T>& function);
    template <typename R>
    StaticArray<R, S> transform(const TransformFunction<R, T>& function) const;

    template <typename R>
    R reduce(const ReduceFunction<R, T>& function, const R& startingValue);
    template <typename R>
    R reduce(const ReduceFunction<R, T>& function, const R& startingValue) const;

    void print(std::ostream& os,
               const std::string& prefix  = "[ ",
               const std::string& infix   = " ",
               const std::string& postfix = " ]");
    
    template <typename T2, std::size_t S2>
    auto operator<=>(const StaticArray<T2,S2>& other) const;

    StaticArray(const T& defaultValue = T());
    StaticArray(T* array, std::size_t size, const T& defaultValue = T());
    StaticArray(std::initializer_list<T> list, const T& defaultValue = T());

    ~StaticArray() = default;

private:
    T m_array[S];
};

static_assert(nitron::IterableClass<nitron::StaticArray<int,10>>);

} // namespace nitron

#include "StaticArray.tpp"