#pragma once

#include "Table.hpp"

namespace nitron
{

template <Streamable Type>
std::unique_ptr<IDataHolder> DataHolder<Type>::clone() const
{
    return std::make_unique<DataHolder<Type>>(m_data);
}

template <Streamable Type>
std::type_index DataHolder<Type>::getType() const
{
    return typeid(Type);
}

template <Streamable Type>
std::string DataHolder<Type>::getString() const
{
    std::stringstream str;
    str << m_data;
    return str.str();
}

template <Streamable Type>
void DataHolder<Type>::setString(std::string const& string)
{
    std::stringstream str;
    str << string;
    str >> m_data;
    if (str.fail())
    {
        throw std::invalid_argument("type of string received from DataHolder<Type>::setString did not match the type Type.");
    }
}

template <Streamable Type>
Type const& DataHolder<Type>::get() const
{
    return m_data;
}

template <Streamable Type>
void DataHolder<Type>::set(Type&& value)
{
    m_data = std::forward<Type>(value);
}

template <Streamable Type>
template <typename... Args>
DataHolder<Type>::DataHolder(Args&&... args)
    : m_data(std::forward<Args>(args)...)
{}

template <Streamable Type>
void Record::pushBackField(Type&& value)
{
    m_cells.emplace_back(
        std::make_unique<DataHolder<Type>>(
            std::forward<Type>(value)
        )
    );
}

template <Streamable Type>
void Record::pushFrontField(Type&& value)
{
    m_cells.emplace_front(
        std::make_unique<DataHolder<Type>>(
            std::forward<Type>(value)
        )
    );
}

template <Streamable Type>
void Record::insertField(size_t index, Type&& value)
{
    m_cells.emplace(m_cells.cbegin() + index,
        std::make_unique<DataHolder<Type>>(
            std::forward<Type>(value)
        )
    );
}

template <Streamable Type>
Type const& Record::get(size_t index) const
{
    IDataHolder const& cell = *m_cells.at(index);
    if (typeid(Type) != cell.getType())
    {
        throw std::invalid_argument("type expected of field from Record::get<Type>() did not match type of data inside DataHolder");
    }
    DataHolder<Type> const& cellTyped = static_cast<DataHolder<Type> const&>(cell);
    return cellTyped.get();
}

template <Streamable Type>
void Record::set(size_t index, Type&& value)
{
    IDataHolder const& cell = *m_cells.at(index);
    if (typeid(Type) != cell.getType())
    {
        throw std::invalid_argument("type expected of field from Record::get<Type>() did not match type of data inside DataHolder");
    }
    DataHolder<Type> const& cellTyped = static_cast<DataHolder<Type> const&>(cell);
    cellTyped.set(std::forward<Type>(value));
}

template <Streamable Type>
void Table::pushBackField(std::string const& header, Type const& defaultValue)
{
    m_headers.emplace_back(header);
    m_prototypes.pushBackField(Type(defaultValue));
    for (Record& record : m_records)
    {
        record.pushBackField(Type(defaultValue));
    }
}

template <Streamable Type>
void Table::pushFrontField(std::string const& header, Type const& defaultValue)
{
    m_headers.emplace_front(header);
    m_prototypes.pushFrontField(Type(defaultValue));
    for (Record& record : m_records)
    {
        record.pushFrontField(Type(defaultValue));
    }
}

template <Streamable Type>
void Table::insertField(size_t index, std::string const& header, Type const& defaultValue)
{
    m_headers.emplace(m_headers.cbegin() + index, header);
    m_prototypes.insertField(index, Type(defaultValue));
    for (Record& record : m_records)
    {
        record.insertField(index, Type(defaultValue));
    }
}

}