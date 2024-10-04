#pragma once

#include "Erase.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeList {

/// @brief Erases types that satisfy a predicate
/// @tparam Ph - placeholder type
/// @tparam Pred - template class predicate
/// @tparam Pos - where to start erasing
template<class Ph, template<class> class Pred, size_t Pos>
struct PhEraseIf {
	using type = Ph;
};

template<template<class> class Pred, class T, class... Ts>
struct PhEraseIf<_Ph<T, Ts...>, Pred, 0> {
private:

	using head = std::conditional_t<Pred<T>::value, _Ph<>, _Ph<T>>;
	using tail = PhEraseIf<_Ph<Ts...>, Pred, 0>::type;

public:

	using type = PhCat<true, head, tail>::type;
};

template<template<class> class Pred, class T, class... Ts, size_t Pos>
struct PhEraseIf<_Ph<T, Ts...>, Pred, Pos> {
private:

	using head = _Ph<T>;
	using tail = PhEraseIf<_Ph<Ts...>, Pred, Pos - 1>::type;

public:

	using type = PhCat<true, head, tail>::type;
};

/// @brief Predicate comparing types
/// @tparam T - type to compare with
template<class T>
struct PhIsSameAs {
	/// @brief Actual predicate
	/// @tparam T2 - compared type
	template<class T2>
	struct pred {
		static inline constexpr bool value = std::same_as<T, T2>;
	};
};

/// @brief Erases given type from placeholder
/// @tparam Ph - placeholder type
/// @tparam T - type to erase
/// @tparam Pos - where to start erasing
template<class Ph, class T, size_t Pos>
struct PhEraseT {
	using type = typename PhEraseIf<Ph, PhIsSameAs<T>::template pred, Pos>::type;
};

} // namespace arch::tUtils::typeList
