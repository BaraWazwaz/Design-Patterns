/**
 * @file
 */

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include "factory.hpp"

namespace nitron
{
    template <Stringifiable T>
    Data<T>::Data(const Data<T>::ValueType& _value) :
        value(_value)
    {}
    
    template <Stringifiable T>
    std::type_index Data<T>::getType()
    {
        return typeid(ValueType);
    }
    
    template <Stringifiable T>
    const std::type_index Data<T>::getType() const
    {
        return typeid(ValueType);
    }

    template <Stringifiable T>
    const std::string Data<T>::get() const
    {
        if constexpr (std::is_same_v<T, std::string>)
            return value;
        std::stringstream str;
        str << value;
        return str.str();
    }

    template <Stringifiable T>
    std::string Data<T>::get()
    {
        if constexpr (std::is_same_v<T, std::string>)
            return value;
        std::stringstream str;
        str << value;
        return str.str();
    }

    template <Stringifiable T>
    void Data<T>::set(const std::string& replacement)
    {
        ValueType* pointer = static_cast<ValueType*>(DataAdapter::create(getType(), replacement));
        if (pointer != nullptr)
        {
            value = *pointer;
            delete pointer;
        }
    }
} // namespace nitron