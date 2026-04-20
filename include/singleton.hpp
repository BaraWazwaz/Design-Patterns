/**
 * @file
 * @brief Class that implements Design Pattern known as @ref Singleton.
 */
#pragma once
#include <concepts>

namespace nitron
{

template <typename T>
concept ProperSingleton = !std::is_default_constructible_v<T> &&
                          !std::is_copy_constructible_v<T> &&
                          !std::is_move_constructible_v<T> &&
                          !std::is_copy_assignable_v<T> &&
                          !std::is_move_assignable_v<T>;

template <typename T>
class Singleton
{
public:
    using ValueType = T;

    static_assert(ProperSingleton<ValueType>,
                  "Wrapped type is NOT suitable to be a Singleton.");

    Singleton()                            = delete;
    Singleton(const Singleton&)            = delete;
    Singleton(Singleton&&)                 = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&)      = delete;
    
    static ValueType& get();
};

} // namespace nitron

#include "Singleton.tpp"