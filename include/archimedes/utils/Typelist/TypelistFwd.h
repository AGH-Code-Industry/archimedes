#pragma once

namespace arch::utils {

/// @brief Type collection, allowing for manipulation of type sequences
/// @tparam Types... - stored types
template<class... Types>
struct Typelist;

/// @brief Helper instance of Typelist
/// @tparam Types... - stored types
template<class... Types>
static inline constexpr const Typelist<Types...> typelist = {};

/// @brief Concatenates typelists
/// @param other - typelist to append
template<class... Types1, class... Types2>
consteval auto operator+(Typelist<Types1...> lhs, Typelist<Types2...> rhs) {
	return Typelist<Types1..., Types2...>();
}

} // namespace arch::utils
