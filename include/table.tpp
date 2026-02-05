#pragma once

namespace nitron
{
    template <Streamable Type>
    Value<Type>::Value(Type val) : 
        m_data(std::move(val))
    {}

    template <Streamable Type>
    ValuePtr Value<Type>::clone() const 
    {
        return std::make_unique<Value<Type>>(m_data);
    }

    template <Streamable Type>
    std::type_index Value<Type>::getType() const 
    {
        return typeid(Type);
    }

    template <Streamable Type>
    std::string Value<Type>::toString() const 
    {
        if constexpr (std::is_same_v<Type, std::string>) {
            return m_data;
        } else {
            std::ostringstream oss;
            oss << m_data;
            return oss.str();
        }
    }

    template <Streamable Type>
    void Value<Type>::fromString(const std::string& str)
    {
        if constexpr (std::is_same_v<Type, std::string>) {
            m_data = str;
        } else {
            std::istringstream iss(str);
            iss >> m_data;
            if (iss.fail()) {
                throw std::runtime_error("Failed to parse string into type " + std::string(typeid(Type).name()));
            }
        }
    }

    template <Streamable Type>
    void Value<Type>::set(const Type& val) 
    {
        m_data = val;
    }

    template <Streamable Type>
    const Type& Value<Type>::get() const 
    {
        return m_data;
    }

    template <Streamable Type>
    void Record::set(size_t index, const Type& val)
    {
        IValue& cell = getCell(index);
        if (cell.getType() != typeid(Type)) {
            throw std::bad_cast();
        }
        static_cast<Value<Type>&>(cell).set(val);
    }

    template <Streamable Type>
    void Table::addField(const std::string& name, const Type& defaultValue)
    {
        m_headers.push_back(name);
        m_prototypes.push_back(std::make_unique<Value<Type>>(defaultValue));
        for (Record& record : m_records) {
            record.addField(std::make_unique<Value<Type>>(defaultValue));
        }
    }

}