/**
 * @file table.hpp
 * @brief Header file for the Table implementation.
 * 
 * Defines the IValue interface, Value template, Record, and Table classes.
 */

#pragma once

#include <iostream>   // Standard stream I/O
#include <string>     // std::string class
#include <vector>     // std::vector container
#include <memory>     // Smart pointers (std::unique_ptr)
#include <sstream>    // String stream classes
#include <typeinfo>   // Type information
#include <typeindex>  // std::type_index class
#include <concepts>   // Standard concepts
#include <algorithm>  // Standard algorithms
#include <stdexcept>  // Standard exceptions

/**
 * @namespace nitron
 * @brief Main namespace for the Design Pattern project components.
 */
namespace nitron
{

/**
 * @concept Streamable
 * @brief Concept ensuring a type satisfies input/output stream operations.
 * @tparam Type The type to check against the concept.
 * 
 * Requires the type to support operator<< (stream insertion) and operator>> (stream extraction).
 */
template <typename Type>
concept Streamable = requires(std::ostream& os, 
                                std::istream& is, 
                                Type& val, 
                                const Type& c_val)
{
    { os << c_val };
    { is >> val };
};

/**
 * @interface IValue
 * @brief Interface for type-erased value wrappers.
 * 
 * Provides a common interface for polymorphic handling of heterogenous data types.
 */
class IValue
{
public:
    /// @brief Virtual destructor.
    virtual ~IValue() = default;

    /**
     * @brief Creates a deep copy of the value.
     * @return std::unique_ptr<IValue> Pointer to the cloned object.
     */
    [[nodiscard]] virtual std::unique_ptr<IValue> clone() const = 0;

    /**
     * @brief Retrieves the type index of the stored value.
     * @return std::type_index Unique identifier for the type.
     */
    [[nodiscard]] virtual std::type_index getType() const = 0;
    
    /**
     * @brief Converts the stored value to a string representation.
     * @return std::string String representation of the value.
     */
    [[nodiscard]] virtual std::string toString() const = 0;

    /**
     * @brief Parses a string to update the stored value.
     * @param str The string to parse.
     * @throws std::runtime_error if parsing fails.
     */
    virtual void fromString(const std::string& str) = 0;
};

/// @brief Alias for a unique pointer to an IValue.
using ValuePtr = std::unique_ptr<IValue>;

/**
 * @class Value
 * @brief Concrete template implementation of IValue.
 * @tparam Type The underlying data type, must be Streamable.
 * 
 * Wraps a specific type and implements polymorphic operations.
 */
template <Streamable Type>
class Value final : public IValue
{
public:
    /**
     * @brief Constructor.
     * @param val Initial value (defaults to default-constructed Type).
     */
    explicit Value(Type val = {});

    // Inherited documentation
    [[nodiscard]] std::unique_ptr<IValue> clone() const override;

    // Inherited documentation
    [[nodiscard]] std::type_index getType() const override;

    // Inherited documentation
    [[nodiscard]] std::string toString() const override;

    // Inherited documentation
    void fromString(const std::string& str) override;

    /**
     * @brief Sets the underlying value directly.
     * @param val The new value to store.
     */
    void set(const Type& val);

    /**
     * @brief Gets the underlying value directly.
     * @return const Type& Reference to the stored value.
     */
    const Type& get() const;

private:
    Type m_data; ///< The actual stored data.
};

/**
 * @class Record
 * @brief Represents a single row in the @ref Table.
 * 
 * A record consists of a collection of values (cells).
 */
class Record
{
public:
    /**
     * @brief Adds a new field (cell) to the record.
     * @param value Pointer to the value to add.
     * @throws std::invalid_argument if value is null.
     */
    void addField(ValuePtr value);

    /**
     * @brief Removes the last field from the record.
     * Does nothing if the record is empty.
     */
    void popField();

    /**
     * @brief Removes a field at a specific index.
     * @param index Index of the field to remove.
     * @throws std::out_of_range if index is invalid.
     */
    void eraseField(size_t index);

    /**
     * @brief Gets the string representation of a cell's value.
     * @param index Index of the cell.
     * @return std::string String representation.
     * @throws std::out_of_range if index is invalid.
     */
    [[nodiscard]] std::string getString(size_t index) const;

    /**
     * @brief Sets a cell's value from a string.
     * @param index Index of the cell.
     * @param val String to parse.
     * @throws std::out_of_range if index is invalid.
     * @throws std::runtime_error if parsing fails.
     */
    void setString(size_t index, const std::string& val);

    /**
     * @brief Type-safe setter for a cell's value.
     * @tparam Type The expected type of the cell.
     * @param index Index of the cell.
     * @param val The new value.
     * @throws std::out_of_range if index is invalid.
     * @throws std::bad_cast if Type does not match the cell's type.
     */
    template <Streamable Type>
    void set(size_t index, const Type& val);

    /**
     * @brief Returns the number of fields in the record.
     * @return size_t Count of fields.
     */
    [[nodiscard]] size_t size() const;

private:
    /**
     * @brief Helper to access a specific cell.
     * @param index Index of the cell.
     * @return IValue& Reference to the cell value.
     * @throws std::out_of_range if index is invalid.
     */
    IValue& getCell(size_t index);

    /**
     * @brief Helper to access a specific cell (const).
     * @param index Index of the cell.
     * @return const IValue& Const reference to the cell value.
     * @throws std::out_of_range if index is invalid.
     */
    const IValue& getCell(size_t index) const;

    std::vector<ValuePtr> m_cells; ///< Collection of cells.
    
    friend class Table; ///< Grant access to Table (optimized for cloning).
};

/**
 * @class Table
 * @brief A dynamic table structure capable of holding heterogenous data.
 * 
 * The table maintains a set of columns (headers and prototypes) and a collection of records.
 */
class Table
{
public:
    /**
     * @brief Adds a new column to the table.
     * @tparam Type The type of data this column will hold.
     * @param name Name of the column header.
     * @param defaultValue Default value for existing and new rows.
     */
    template <Streamable Type>
    void addField(const std::string& name, const Type& defaultValue = {});

    /**
     * @brief Removes the last column/field from the table.
     * Removes the header, prototype, and the corresponding cell from every record.
     */
    void removeLastField();

    /**
     * @brief Removes a specific column/field from the table.
     * Removes the header, prototype, and the corresponding cell from every record at the given index.
     * @param index Index of the column to remove.
     * @throws std::out_of_range if index is invalid.
     */
    void removeField(size_t index);

    /**
     * @brief Adds a new row to the table.
     * Populates the new row with default values cloned from column prototypes.
     */
    void addRow();

    /**
     * @brief Accesses a specific row.
     * @param index Index of the row.
     * @return Record& Reference to the record.
     * @throws std::out_of_range if index is invalid.
     */
    Record& getRow(size_t index);

    /**
     * @brief Accesses a specific row (const).
     * @param index Index of the row.
     * @return const Record& Const reference to the record.
     * @throws std::out_of_range if index is invalid.
     */
    const Record& getRow(size_t index) const;

    /// @brief Returns the number of rows (records).
    [[nodiscard]] size_t rowCount() const;

    /// @brief Returns the number of columns (fields).
    [[nodiscard]] size_t colCount() const;
    
    /**
     * @brief Gets the name of a column header.
     * @param index Index of the column.
     * @return const std::string& Name of the header.
     * @throws std::out_of_range if index is invalid.
     */
    const std::string& getHeader(size_t index) const;

    /**
     * @brief Sets the name of a column header.
     * @param index Index of the column.
     * @param header New name for the header.
     * @throws std::out_of_range if index is invalid.
     */
    void setHeader(size_t index, const std::string& header);

    /**
     * @brief Stream output operator for Table.
     * Prints headers and all records to the output stream.
     * @param os Output stream.
     * @param table Table to print.
     * @return std::ostream& The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Table& table);

private:
    std::vector<std::string> m_headers;   ///< Column names.
    std::vector<ValuePtr> m_prototypes;   ///< Prototypes for value cloning.
    std::vector<Record> m_records;        ///< Data rows.
};

}

#include "Table.tpp"