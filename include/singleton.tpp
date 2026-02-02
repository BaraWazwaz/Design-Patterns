/**
 * @file
 * @brief Implementation of the @ref Singleton.hpp header for the @ref Singleton class.
 */

#include "singleton.hpp"

namespace nitron
{
    template <typename T>
    Singleton<T>::ValueType& Singleton<T>::get()
    {
        static ValueType value;
        return value;
    }
} // namespace nitron
