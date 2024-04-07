#pragma once

#include <type_traits>

namespace arch::meta::_typeof {
/// @brief Helper struct of noref_typeof() macro
/// @brief (does nothing)
struct TypeOfHelper {
	/// @brief Helper casting operator
	/// @brief (does nothing)
	template<class T>
	operator T() const;
	/// @brief Helper dereference operator
	/// @brief (does nothing)
	TypeOfHelper operator*();
};

/// @brief Helper multiplication operator
/// @brief (does nothing)
template<class T>
T operator*(T a, TypeOfHelper);
} // namespace arch::meta::_typeof

// evil operator*/casting semantic level hacking
#define _typeofImpl(x) std::remove_cvref_t<decltype((x) * arch::meta::_typeof::TypeOfHelper{})>

/// @brief Queries non-reference type of expression or type given
/// @param x - type or expression to query
#define typeof(x) _typeofImpl(x)
