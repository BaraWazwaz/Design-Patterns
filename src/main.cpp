#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "factory.hpp"

using nitron::DataAdapter;
using nitron::DataHolder;
using nitron::makeDataHolder;

class Record
{
public:

    inline void emplace_back(DataHolder&& dataHolder)
    {
        data.emplace_back(std::move(dataHolder));
    }

    inline void emplace_back(DataHolder& dataHolder)
    {
        data.emplace_back(std::move(dataHolder));
    }

    inline void pop_back()
    {
        data.pop_back();
    }

    inline std::string read(size_t index)
    {
        return data.at(index)->get();
    }
    
    inline const std::string read(size_t index) const
    {
        return data.at(index)->get();
    }

    inline DataHolder& at(size_t index)
    {
        return data.at(index);
    }

    inline const DataHolder& at(size_t index) const
    {
        return data.at(index);
    }

    template <typename T>
    inline void set(size_t index, const T& value)
    {
        DataHolder& oldDataHolder = data.at(index);
        if (typeid(T) != typeid(*oldDataHolder))
            return;
        DataHolder newDataHolder = makeDataHolder(nitron::Data<T>(value));
        oldDataHolder.swap(newDataHolder);
    }
    
    inline size_t size()
    {
        return data.size();
    }
    
    inline const size_t size() const
    {
        return data.size();
    }
private:
    std::vector<DataHolder> data;
};

class Table 
{
public:

    inline void addRecord()
    {
        data.emplace_back();
        for (size_t fieldIndex = 0; fieldIndex < prototype.size(); ++fieldIndex)
            data.back().emplace_back(makeDataHolder(*prototype.at(fieldIndex)));
    }

    template <typename T>
    inline void addColumnToBack(const std::string& title, const T& defaultValue = T())
    {
        titles.emplace_back(title);
        prototype.emplace_back(makeDataHolder(nitron::Data<T>()));
        for (Record& record : data)
        {
            record.emplace_back(makeDataHolder(nitron::Data<T>(defaultValue)));
        }
    }

    inline void deleteLastColumn()
    {
        titles.pop_back();
        prototype.pop_back();
        for (Record& record : data)
        {
            record.pop_back();
        }
    }

    inline Record& getRecord(size_t index)
    {
        return data.at(index);
    }

    inline const Record& getRecord(size_t index) const
    {
        return data.at(index);
    }

    inline std::type_index getColumnType(size_t index)
    {
        return typeid(*prototype.at(index));
    }

    inline const std::type_index getColumnType(size_t index) const
    {
        return typeid(*prototype.at(index));
    }

    inline size_t recordCount()
    {
        return data.size();
    }

    inline const size_t recordCount() const
    {
        return data.size();
    }

    inline size_t fieldSize()
    {
        return titles.size();
    }

    inline const size_t fieldSize() const
    {
        return titles.size();
    }

    friend std::ostream& operator<<(std::ostream& out, const Table& table)
    {
        out << "Fields:" << '\n';
        for (size_t fieldIndex = 0; fieldIndex < table.fieldSize(); ++fieldIndex)
        {
            out << '\t' << table.titles.at(fieldIndex) << '\n';
        }
        out << '\n';
        for (size_t recordIndex = 0; recordIndex < table.recordCount(); ++recordIndex)
        {
            out << "Record #" << recordIndex << '\n';
            const Record& record = table.getRecord(recordIndex);
            for (size_t fieldIndex = 0; fieldIndex < table.fieldSize(); ++fieldIndex)
            {
                out << '\t' << table.titles.at(fieldIndex) << '=' << record.read(fieldIndex) << '\n';
            }
        }
        return out;
    }

private:
    std::vector<std::string> titles;
    Record prototype;
    std::vector<Record> data;
};

int main()
{
    Table table;
    table.addColumnToBack<int>("id", 0);
    table.addRecord();
    table.getRecord(0).set(0, 100);
    std::cout << table;
    return 0;
}