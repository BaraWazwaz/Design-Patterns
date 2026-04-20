#include "Singleton.hpp"

namespace nitron
{

template <typename T>
Singleton<T>::ValueType& Singleton<T>::get()
{
    static ValueType value;
    return value;
}

}