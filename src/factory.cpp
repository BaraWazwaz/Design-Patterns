/**
 * @file
 */

#include <functional>
#include <string>
#include <sstream>
#include <unordered_map>
#include <memory>
#include "factory.hpp"

namespace nitron
{
    std::type_index DataAdapter::getType()
    {
        return typeid(void);
    }

    const std::type_index DataAdapter::getType() const
    {
        return typeid(void);
    }

    std::string DataAdapter::get()
    {
        return "Null";
    }
        
    const std::string DataAdapter::get() const
    {
        return "Null";
    }

    void DataAdapter::set(const std::string& replacement)
    {
    }

    void* DataAdapter::create(const std::type_index& type, const std::string& value)
    {
        using Constructor = std::function<void*(const std::string&)>;
        static auto makeConstructor = [](auto dummy) -> std::pair<std::type_index, Constructor>
        {
            using T = decltype(dummy);
            return
            {
                std::type_index(typeid(T)),
                [](const std::string& value) -> void* {
                    std::stringstream str (value);
                    str.exceptions(std::ios_base::failbit | std::ios_base::badbit);
                    T* extracted = new T();
                    try 
                    {
                        str >> (*extracted);
                    }
                    catch(const std::ios_base::failure& e)
                    {
                        std::cerr << "Encountered an I/O Exception, code:" << e.code() << '\n';
                        std::cerr << "Error message:" << '\n';
                        std::cerr << e.what() << std::endl;
                        return nullptr;
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "Encountered a General Exception" << '\n';
                        std::cerr << "Error message:" << '\n';
                        std::cerr << e.what() << std::endl;
                        return nullptr;
                    }
                    return extracted;
                }
            };
        };
        static std::unordered_map<std::type_index, Constructor> mapper =
        {
            makeConstructor((int)(0)),
            makeConstructor((short)(0)),
            makeConstructor((long)(0)),
            makeConstructor((long long)(0)),
            makeConstructor((unsigned)(0)),
            makeConstructor((unsigned short)(0)),
            makeConstructor((unsigned long)(0)),
            makeConstructor((unsigned long long)(0)),
            makeConstructor((float)(0)),
            makeConstructor((double)(0)),
            makeConstructor((long double)(0)),
            makeConstructor(std::string(""))
        };
        auto it = mapper.find(type);
        if (it == mapper.end())
            return nullptr;
        else
            return it->second(value);
    }

    DataHolder makeDataHolder(const DataAdapter& dataAdapter)
    {
        return std::make_unique<DataAdapter>(dataAdapter);
    };
} // namespace nitron
