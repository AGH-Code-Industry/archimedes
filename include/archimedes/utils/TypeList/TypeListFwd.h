#pragma once

namespace arch::utils {

/// @brief Type collection, allowing for manipulation of type sequences
/// @tparam Types... - stored types
template<class... Types>
struct TypeList;

/// @brief Helper instance of TypeList
/// @tparam Types... - stored types
template<class... Types>
static inline constexpr const TypeList<Types...> typelist = {};

/// @brief Concatenates typelists
/// @param other - typelist to append
template<class... Types1, class... Types2>
consteval auto operator+(TypeList<Types1...> lhs, TypeList<Types2...> rhs) {
	return TypeList<Types1..., Types2...>();
}

} // namespace arch::utils
