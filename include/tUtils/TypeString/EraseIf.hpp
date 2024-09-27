#pragma once

#include "Erase.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeString {
template<class Ph, template<class PT> class Pred, size_t Pos>
struct PhEraseIf {
	using type = Ph;
};

template<template<class PT> class Pred, class T, class... Ts>
struct PhEraseIf<_Ph<T, Ts...>, Pred, 0> {
	using head = CondT<Pred<T>::value, _Ph<>, _Ph<T>>;
	using tail = PhEraseIf<_Ph<Ts...>, Pred, 0>::type;

	using type = PhCat<true, head, tail>::type;
};

template<class T>
struct xd {
	static inline constexpr bool value = false;
};

template<template<class PT> class Pred, class T, class... Ts, size_t Pos>
struct PhEraseIf<_Ph<T, Ts...>, Pred, Pos> {
	template<class T2>
	struct pred {
		// public:
		static inline constexpr bool value = std::same_as<T, T2>;
	};

	using head = _Ph<T>;
	using tail = PhEraseIf<_Ph<Ts...>, Pred, Pos - 1>::type;

	using type = PhCat<true, head, tail>::type;
};

template<class T>
struct phIsSameAs {
	template<class T2>
	struct pred {
		static inline constexpr bool value = std::same_as<T, T2>;
	};
};

template<class Ph, class T, size_t Pos>
struct PhEraseT {
	using type = typename PhEraseIf<Ph, phIsSameAs<T>::template pred, Pos>::type;
};
} // namespace arch::tUtils::typeString
