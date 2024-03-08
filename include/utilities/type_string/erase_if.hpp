#pragma

#include "placeholder.hpp"
#include "erase.hpp"

namespace arch::ts {
template<class Ph, template<class T> class Pred, size_t Pos>
struct ph_erase_if {
	using type = Ph;
};

template<template<class PT> class Pred, class T, class... Ts>
struct ph_erase_if<ph<T, Ts...>, Pred, 0> {
	using head = cond_t<Pred<T>::value, ph<>, ph<T>>;
	using tail = ph_erase_if<ph<Ts...>, Pred, 0>::type;

	using type = ph_cat<true, head, tail>::type;
};

template<template<class PT> class Pred, class T, class... Ts, size_t Pos>
struct ph_erase_if<ph<T, Ts...>, Pred, Pos> {
	using head = ph<T>;
	using tail = ph_erase_if<ph<Ts...>, Pred, Pos - 1>::type;

	using type = ph_cat<true, head, tail>::type;
};

template<class T>
struct ph_is_same_as {
	template<class T2>
	struct pred {
		static inline constexpr bool value = std::same_as<T, T2>;
	};
};

template<class Ph, class T, size_t Pos>
struct ph_erase_t {
	using type = ph_erase_if<Ph, ph_is_same_as<T>::pred, Pos>::type;
};
}