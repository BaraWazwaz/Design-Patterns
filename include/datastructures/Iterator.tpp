#include "Iterator.hpp"

#include <stdexcept>

namespace nitron
{

template <typename T>
Iterator<T>::Iterator(T* pointer) :
    pointer(pointer)
{}

template <typename T>
T* Iterator<T>::get() const
{
    return pointer;
}

template <typename T>
Iterator<T>::operator T*() const
{
    return pointer;
}

template <typename T>
Iterator<T>& Iterator<T>::operator++()
{
    ++pointer;
    return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator++(int)
{
    Iterator result = *this;
    ++pointer;
    return result;
}

template <typename T>
Iterator<T>& Iterator<T>::operator--()
{
    --pointer;
    return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator--(int)
{
    Iterator result = *this;
    --pointer;
    return result;
}

template <typename T>
T& Iterator<T>::operator*() const
{
    if (pointer == nullptr)
    {
        throw std::runtime_error("nitron::Iterator operator*() : cannot dereference null pointer.");
    }
    return *pointer;
}

template <typename T>
T* Iterator<T>::operator->() const
{
    if (pointer == nullptr)
    {
        throw std::runtime_error("nitron::Iterator operator*() : cannot dereference null pointer.");
    }
    return pointer;
}

} // namespace nitron
