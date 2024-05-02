#pragma once

#include <type_traits>

namespace arch::meta::typeOf::_details {
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
} // namespace arch::meta::typeOf::_details

// if ... is a type			-> dereference Helper, cast to ..., obtain decltype of result
// if ... is an expression	-> multiply ... by Helper, obtain decltype of result
#define _ARCH_TYPEOF_IMPL(...)                           \
	/* evil operator* and cast semantic level hacking */ \
	std::remove_cvref_t<decltype((__VA_ARGS__) * arch::meta::typeOf::_details::Helper{})>

/// @brief Queries non-const, non-reference type of expression or type given
/// @param x - type or expression to query
#define typeof(...) _ARCH_TYPEOF_IMPL(__VA_ARGS__)
