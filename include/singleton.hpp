/**
 * @file
 * @brief Class that implements Design Pattern known as @ref Singleton.
 */
#pragma once
#include <concepts>

namespace nitron
{
    /**
     * @brief Concept that makes sure the wrapper class is a proper Singleton.
     * @tparam T Type to check compliance with Singleton properties
     * @details a class has to be :
     * - Not Default Constructable.
     * - Not Copy Constructable.
     * - Not Move Constructable.
     * - Not Copy Assignable.
     * - Not Move Assignable.
     */
    template <typename T>
    concept ProperSingleton = !std::is_default_constructible_v<T> &&
                              !std::is_copy_constructible_v<T> &&
                              !std::is_move_constructible_v<T> &&
                              !std::is_copy_assignable_v<T> &&
                              !std::is_move_assignable_v<T>;

    /**
     * @brief Singleton class wrapper.
     * @tparam T Type to be wrapped as Singleton,
     *  a class has to be :
     * - Privately Default Constructable.
     * - Not Copy Constructable.
     * - Not Move Constructable.
     * - Not Copy Assignable.
     * - Not Move Assignable.
     * @details @ref Singleton Design Pattern allows only one instance of class, 
     * to limit and control access to the wrapped class's constructor.
     * 
     * More found at <https://refactoring.guru/design-patterns/singleton>.
     */
    template <typename T>
    class Singleton
    {
    public:
        using ValueType = T;    ///< Member Type to Access templated wrapped class.

        static_assert(ProperSingleton<ValueType>, 
                      "Wrapped type is NOT suitable to be a Singleton.");

        Singleton()                            = delete;    ///< Pervent Default Construction.
        Singleton(const Singleton&)            = delete;    ///< Prevent Copy Construction.
        Singleton(Singleton&&)                 = delete;    ///< Prevent Move Construction.
        Singleton& operator=(const Singleton&) = delete;    ///< Prevent Copy Assingment.
        Singleton& operator=(Singleton&&)      = delete;    ///< Prevent Move Assingment.
        
        /**
         * @brief Access to instance.
         * @return a Reference to the instance.
         */
        static ValueType& get();
    };
    
} // namespace nitron

#include "singleton.tpp"