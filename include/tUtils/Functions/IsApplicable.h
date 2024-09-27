#pragma once

#include <tuple>
#include <type_traits>

namespace arch::tUtils {

/// @brief Type trait, based on std::is_invocable
/// @tparam Fn - function type
/// @tparam Tuple - tuple of arguments to pass to function
template<class Fn, class Tuple>
struct IsApplicable: public std::false_type {};

/// @brief Type trait, based on std::is_invocable
/// @tparam Fn - function type
/// @tparam Args - arguments to pass to function
template<class Fn, class... Args>
struct IsApplicable<Fn, std::tuple<Args...>>: public std::is_invocable<Fn, Args...> {};

/// @brief Helper value of IsApplicable
/// @tparam Fn - function type
/// @tparam Tuple - tuple of arguments to pass to function
template<class Fn, class Tuple>
static inline constexpr bool isApplicableV = IsApplicable<Fn, Tuple>::value;

} // namespace arch::tUtils
