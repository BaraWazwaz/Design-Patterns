/**
 * @file DataStructures.hpp
 * @brief Main header of library data structures.
 */

#pragma once

#include <functional>
#include "Tag.hpp"
#include "utilities.hpp"

namespace nitron {

struct DataStructureTag : virtual public Tag {};

template <typename T>
concept DataStructureClass = requires(T x)
{
    typename T::Info;
    requires std::is_base_of_v<DataStructureTag, typename T::Info>;
    
    { x.size() } -> std::same_as<std::size_t>;
    { x.clear() } -> std::same_as<void>;
    { x.empty() } -> std::same_as<bool>;
};

struct IteratorTag : virtual public Tag {};

template <typename T>
concept IteratorClass = requires(T x)
{
    typename T::Info;
    requires std::is_base_of_v<IteratorTag, typename T::Info>;

    { ++x } -> std::same_as<T&>;
    { --x } -> std::same_as<T&>;
    { x++ } -> std::same_as<T>;
    { x-- } -> std::same_as<T>;
    { *x } -> DifferentTypes<void>;
    requires std::is_reference_v<decltype(*x)>;
};

struct HomogeneousDSTag : virtual public DataStructureTag {};

template <typename T>
concept HomogeneousDSClass = requires(T x)
{
    requires DataStructureClass<T>;
    requires std::is_base_of_v<HomogeneousDSTag, typename T::Info>;
    
    typename T::Info::KeyType;
    typename T::Info::ValueType;
    { x.at(std::declval<const typename T::Info::KeyType&>()) } -> std::same_as<typename T::Info::ValueType&>;
};

struct IterableTag : virtual public HomogeneousDSTag {};

template <typename T>
concept IterableClass = requires()
{
    requires HomogeneousDSClass<T>;
    requires std::is_base_of_v<IterableTag, typename T::Info>;
    
    typename T::Info::Iterator;
    // typename T::Info::IteratorConst;
    requires IteratorClass<typename T::Info::Iterator>;
    // requires IteratorClass<typename T::Info::IteratorConst>;

    requires std::convertible_to<decltype(*typename T::Info::Iterator()), typename T::Info::ValueType&>;
};

struct LinearDSTag : virtual public HomogeneousDSTag, virtual public IterableTag {};

template <typename T>
concept LinearDSClass = requires(T x)
{
    requires HomogeneousDSClass<T>;
    requires IterableClass<T>;
    requires std::is_base_of_v<LinearDSTag, typename T::Info>;

    { x.begin() } -> std::same_as<typename T::Info::Iterator>;
    { x.end() } -> std::same_as<typename T::Info::Iterator>;
    { x.forEach(std::declval<ForEachFunction<typename T::Info::ValueType>>()) } -> std::same_as<void>;
    { x.transform(std::declval<TransformFunction<typename T::Info::ValueType,typename T::Info::ValueType>>()) } -> std::same_as<T>;
    { x.reduce(std::declval<ReduceFunction<typename T::Info::ValueType,typename T::Info::ValueType>>(), typename T::Info::ValueType()) } -> std::same_as<typename T::Info::ValueType>;
};

struct TreeDS        : virtual public HomogeneousDSTag {};
struct GraphDS       : virtual public HomogeneousDSTag {};

struct HeterogeneousDS : virtual public DataStructureTag {};
struct PolymorphicDS   : virtual public HeterogeneousDS {};
struct TupleDS         : virtual public HeterogeneousDS {};

} // namespace nitron