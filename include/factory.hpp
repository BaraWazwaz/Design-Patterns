/**
 * @file
 */

#pragma once
#include <string>
#include <sstream>
#include <typeindex>
#include <concepts>
#include <memory>

namespace nitron
{
    class DataAdapter
    {
    public:
        
        virtual ~DataAdapter() = default;
        
        virtual std::type_index getType();

        virtual const std::type_index getType() const;
        
        virtual std::string get();

        virtual const std::string get() const;
        
        virtual void set(const std::string& replacement);

    protected:

        static void* create(const std::type_index& type, const std::string& value);
    };

    template <typename T>
    concept Stringifiable =
        std::is_same_v<T, std::string> ||
        requires(T x, std::stringstream str)
        {
            { str >> x };
        };

    template <Stringifiable T>
    class Data : public DataAdapter
    {
    public:
        using ValueType = T;

        Data()                       = default;
        Data(const Data&)            = default;
        Data(Data&&)                 = default;
        Data& operator=(const Data&) = default;
        Data& operator=(Data&&)      = default;

        Data(const ValueType& _value);

        std::type_index getType() override;

        const std::type_index getType() const override;

        std::string get() override;

        const std::string get() const override;
        
        void set(const std::string& replacement) override;

    private:
        ValueType value;
    };

    typedef std::unique_ptr<DataAdapter> DataHolder;
    
    
    DataHolder makeDataHolder(const DataAdapter& dataAdapter);

} // namespace nitron

#include "factory.tpp"