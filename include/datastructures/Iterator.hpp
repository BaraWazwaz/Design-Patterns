#pragma once

#include "DataStructures.hpp"

namespace nitron
{

template <typename T>
class Iterator
{
    public:
    
    struct Info : IteratorTag
    {
        using ValueType = T;
    };
    
    Iterator(T* pointer = nullptr);
    
    T* get() const;
    operator T*() const;

    auto operator<=>(const Iterator& other) const = default;
    bool operator== (const Iterator& other) const = default;
    bool operator!= (const Iterator& other) const = default;
    bool operator<  (const Iterator& other) const = default;
    bool operator>  (const Iterator& other) const = default;
    bool operator<= (const Iterator& other) const = default;
    bool operator>= (const Iterator& other) const = default;
    
    Iterator& operator++();
    Iterator  operator++(int);
    Iterator& operator--();
    Iterator  operator--(int);
    
    T& operator* () const;
    T* operator->() const;
    
private:
    T* pointer;
};

} // namespace nitron

#include "Iterator.tpp"