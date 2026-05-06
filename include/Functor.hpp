#pragma once

#include <concepts> // std::is_invocable_r_v, std::invocable, std::invoke_result_t, std::convertible_to

namespace nitron
{

/// @brief checks if class is a functor that takes some argument list 
///        and its return value is exactly expected type
template <typename Functor, typename Result, typename... Args>
concept StrictFunctor = std::is_invocable_r_v<Result, Functor, Args...>;

/// @brief checks if class is a functor that takes some argument list 
///        and its return value is convertible to expected type
template <typename Functor, typename Result, typename... Args>
concept LooseFunctor = std::invocable<Functor, Args...> &&
                       std::convertible_to<std::invoke_result_t<Functor, Args...>, Result>;

} // namespace nitron