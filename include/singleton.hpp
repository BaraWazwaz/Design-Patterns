/**
 * @file
 * @brief Class that implements Design Pattern known as @ref Singleton.
 */

#pragma once
#include <memory>

namespace nitron
{
    /**
     * @brief Singleton class wrapper
     * @details @ref Singleton Design Pattern allows only one instance of class, 
     * to limit and control access to the wrapped class's constructor.
     * 
     * More found at <https://refactoring.guru/design-patterns/singleton>.
     */
    template <typename T>
    class Singleton
    {
    public:
        using ValueType = T;
        using PointerType = std::unique_ptr<ValueType>;
        Singleton() = delete;
        /**
         * @brief Writer access to instance.
         * @return a Mutable reference to the instance.
         */
        static ValueType& get();
        /**
         * @brief Reader access to instance.
         * @return a Constant reference to the instance.
         */
        static const ValueType& view();
    private:
        /**
         * @brief Makes sure instance exists before attempting access.
         */
        inline static void render();
        inline static PointerType pointer = nullptr; //!< a Unique pointer to instance.
    };
    
} // namespace nitron

#include "singleton.tpp"