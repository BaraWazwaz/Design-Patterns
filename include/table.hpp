#pragma once

#include <iostream>   // Standard stream I/O
#include <iomanip>    // I/O manipulation (std::setw())
#include <string>     // std::string class
#include <vector>     // std::vector data structure
#include <deque>      // std::deque data structure
#include <memory>     // Smart pointers (std::unique_ptr)
#include <sstream>    // String stream classes
#include <typeindex>  // std::type_index class
#include <concepts>   // Standard concepts
#include <stdexcept>  // Standard exceptions

namespace nitron
{

template <typename Type>
concept Streamable = requires(std::ostream& os, 
                              std::istream& is, 
                              Type vm,
                              Type const& vc)
{
    { os << vc };
    { is >> vm };
};

class IDataHolder
{
public:
    virtual ~IDataHolder() = default;
    [[nodiscard]]
    virtual std::unique_ptr<IDataHolder> clone() const = 0;
    [[nodiscard]]
    virtual std::type_index getType() const = 0;
    [[nodiscard]]
    virtual std::string getString() const = 0;
    virtual void setString(std::string const& str) = 0;
};

template <Streamable Type>
class DataHolder final : public IDataHolder
{
public:
    [[nodiscard]] std::unique_ptr<IDataHolder> clone() const override;
    [[nodiscard]] std::type_index getType() const override;

    [[nodiscard]] std::string getString() const override;
    void setString(std::string const& string) override;

    [[nodiscard]] Type const& getValue() const;
    template <typename... Args>
    void setValue(Args&&... args);

    template <typename... Args>
    explicit DataHolder(Args&&... args);

private:
    Type m_data;
};

class Record
{
public:
    template <Streamable Type, typename... Args>
    void insertField(size_t index, Args&&... value);
    template <Streamable Type, typename... Args>
    void emplaceBackField(Args&&... args);
    template <Streamable Type, typename... Args>
    void emplaceFrontField(Args&&... args);

    void eraseField(size_t index);
    void popBackField();
    void popFrontField();

    [[nodiscard]] IDataHolder& at(size_t index);
    [[nodiscard]] IDataHolder const& at(size_t index) const;

    template <Streamable Type>
    [[nodiscard]] Type const& getFieldValue(size_t index) const;
    template <Streamable Type, typename... Args>
    void setFieldValue(size_t index, Args&&... args);

    [[nodiscard]] std::string getFieldString(size_t index) const;
    void setFieldString(size_t index, std::string const& value);

    [[nodiscard]] size_t degree() const;
    [[nodiscard]] size_t incompatability(Record const& record) const;

    inline static const size_t SIZE_MISMATCH = ULLONG_MAX;

private:
    std::deque<std::unique_ptr<IDataHolder>> m_cells;
};

class Table
{
public:
    template <Streamable Type, typename... Args>
    void emplaceBackField(std::string const& header, Args&&... args);
    template <Streamable Type, typename... Args>
    void emplaceFrontField(std::string const& header, Args&&... args);
    template <Streamable Type, typename... Args>
    void insertField(size_t index, std::string const& header, Args&&... args);

    void popBackField();
    void popFrontField();
    void eraseField(size_t index);

    void emplaceBackRecord(Record&& record);
    void emplaceFrontRecord(Record&& record);
    void insertRecord(size_t index, Record&& record);

    void popBackRecord();
    void popFrontRecord();
    void eraseRecord(size_t index);

    Record& getRecord(size_t index);
    Record const& getRecord(size_t index) const;

    [[nodiscard]] size_t rowCount() const;
    [[nodiscard]] size_t columnCount() const;
    
    [[nodiscard]] std::string const& getHeader(size_t index) const;
    void setHeader(size_t index, std::string const& header);

    inline static const size_t MAX_FIELD_WIDTH = 12;
    
private:
    void assertCompatibility(Record const& record) const;

    std::deque<std::string> m_headers;
    Record m_prototypes;
    std::deque<Record> m_records;
};

std::ostream& operator<<(std::ostream& os, const Table& table);

}

#include "Table.tpp"