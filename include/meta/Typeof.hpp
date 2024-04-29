#pragma once

#include <type_traits>

namespace arch::meta::typeOf {
/// @brief Helper struct of typeof() macro
/// @brief (does nothing)
struct Helper {
	/// @brief Helper casting operator
	/// @brief (does nothing)
	template<class T>
	operator T() const;
	/// @brief Helper dereference operator
	/// @brief (does nothing)
	Helper operator*();
};

/// @brief Helper multiplication operator
/// @brief (does nothing)
template<class T>
const T& operator*(const T&, Helper);
} // namespace arch::meta::typeOf

// if ... is a type			-> dereference Helper, cast to ..., obtain decltype of result
// if ... is an expression	-> multiply ... by Helper, obtain decltype of result
#define _typeofImpl(...)                                 \
	/* evil operator* and cast semantic level hacking */ \
	std::remove_cvref_t<decltype((__VA_ARGS__) * arch::meta::typeOf::Helper{})>

/// @brief Queries non-const, non-reference type of expression or type given
/// @param x - type or expression to query
#define typeof(...) _typeofImpl(__VA_ARGS__)
