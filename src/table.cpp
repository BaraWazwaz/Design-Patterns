#include "table.hpp"

namespace nitron
{
    void Record::addField(ValuePtr value)
    {
        if (!value) throw std::invalid_argument("Cannot add null value");
        m_cells.push_back(std::move(value));
    }

    void Record::popField()
    {
        if (!m_cells.empty()) {
            m_cells.pop_back();
        }
    }

    std::string Record::getString(size_t index) const
    {
        return getCell(index).toString();
    }

    void Record::setString(size_t index, const std::string& val)
    {
        getCell(index).fromString(val);
    }

    size_t Record::size() const { return m_cells.size(); }

    IValue& Record::getCell(size_t index)
    {
        if (index >= m_cells.size()) throw std::out_of_range("Record index out of range");
        return *m_cells[index];
    }

    const IValue& Record::getCell(size_t index) const
    {
        if (index >= m_cells.size()) throw std::out_of_range("Record index out of range");
        return *m_cells[index];
    }

    void Table::removeLastField()
    {
        if (m_headers.empty()) return;

        m_headers.pop_back();
        m_prototypes.pop_back();

        for (auto& record : m_records) {
            record.popField();
        }
    }

    void Table::addRow()
    {
        Record newRecord;
        for (const auto& proto : m_prototypes) {
            newRecord.addField(proto->clone());
        }
        m_records.push_back(std::move(newRecord));
    }

    Record& Table::getRow(size_t index)
    {
        if (index >= m_records.size()) throw std::out_of_range("Row index out of range");
        return m_records[index];
    }

    const Record& Table::getRow(size_t index) const
    {
        if (index >= m_records.size()) throw std::out_of_range("Row index out of range");
        return m_records[index];
    }

    size_t Table::rowCount() const { return m_records.size(); }
    size_t Table::colCount() const { return m_headers.size(); }
    const std::string& Table::getHeader(size_t index) const { return m_headers.at(index); }

    std::ostream& operator<<(std::ostream& os, const Table& table)
    {
        os << "Fields:\n";
        for (const std::string& header : table.m_headers) {
            os << "\t" << header << "\n";
        }
        os << "\n";

        for (size_t i = 0; i < table.m_records.size(); ++i) {
            os << "Record #" << i << "\n";
            const Record& record = table.m_records[i];
            for (size_t j = 0; j < table.m_headers.size(); ++j) {
                os << "\t" << table.m_headers[j] << "=" << record.getString(j) << "\n";
            }
            os << "\n";
        }
        return os;
    }

}