/**
 * @file
 * @brief Class that implements Design Pattern known as @ref Singleton.
 */

#pragma once
#include <memory>

namespace nitron
{
    /**
     * @brief Singleton class wrapper.
     * @tparam T Type to be wrapped as Singleton.
     * @details @ref Singleton Design Pattern allows only one instance of class, 
     * to limit and control access to the wrapped class's constructor.
     * 
     * More found at <https://refactoring.guru/design-patterns/singleton>.
     */
    template <typename T>
    class Singleton
    {
    public:
        using ValueType = T;    ///< Member Type to Access templated wrapped class
        
        Singleton() = delete;                               ///< Pervent Instantiation
        Singleton(const Singleton&) = delete;               ///< Prevent Copying
        Singleton(Singleton&&) = delete;                    ///< Prevent Moving
        Singleton& operator=(const Singleton&) = delete;    ///< Prevent Assingment
        
        /**
         * @brief Access to instance.
         * @return a Reference to the instance.
         */
        static ValueType& get();
    };
    
} // namespace nitron

#include "singleton.tpp"