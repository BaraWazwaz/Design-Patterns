/**
 * @file
 * @brief Implementation of the @ref Singletone.hpp header for the @ref Singleton class.
 */

#include "singleton.hpp"

namespace nitron
{
    template <typename T>
    Singleton<T>::ValueType& Singleton<T>::get()
    {
        render();
        return *pointer;
    }
    template <typename T>
    const Singleton<T>::ValueType& Singleton<T>::view()
    {
        render();
        return *pointer;
    }
    template <typename T>
    void Singleton<T>::render()
    {
        if (pointer == nullptr)
        {
            PointerType pointerWithObject (new ValueType());
            pointer.swap(pointerWithObject);
        }
    }
} // namespace nitron
