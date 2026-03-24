#pragma once

#include "Tag.hpp"

namespace nitron
{

template <typename T1, typename T2>
concept DifferentTypes = !std::same_as<T1, T2>;

template <typename T>
using ForEachFunction = std::function<void(const T&)>;

template <typename R, typename D>
using TransformFunction = std::function<R(const D&)>;

template <typename R, typename D>                
using ReduceFunction = std::function<R(const R&, const D&)>;

} // namespace nitron