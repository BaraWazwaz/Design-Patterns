#pragma once
#include <memory>

namespace nitron
{
    template <typename T>
    class Singleton
    {
    public:
        using ValueType = T;
        using PointerType = std::unique_ptr<ValueType>;
        Singleton() = delete;
        static ValueType& get();
        static const ValueType& view();
    private:
        inline static void render();
        inline static PointerType pointer = nullptr;
    };
    
} // namespace nitron

#include "singleton.tpp"