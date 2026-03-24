/**
 * @file Tag.hpp
 * @brief Provides functionality to define and identify tags using traits.
 * @details This header defines a concept to detect tags through member type identification.
 */
#pragma once

#include <concepts>

namespace nitron
{

/**
 * @brief Base class for defining tags.
 * @details Inherit from this to create a tag.
 * 
 * @code{.cpp}
 * // Defining a tag
 * class MyTag : public nitron::Tag {};
 * 
 * // Using the tag
 * class MyClass
 * {
 * public:
 *     struct Info : public MyTag {}
 * };
 * @endcode
 */
struct Tag
{
    using NitronTag = void;
};

/**
 * @brief Concept to check if a type is a valid Nitron Tag.
 * @tparam T The type to check.
 */
template <typename T>
concept TagClass = requires()
{
    typename T::NitronTag;
};

} // namespace nitron