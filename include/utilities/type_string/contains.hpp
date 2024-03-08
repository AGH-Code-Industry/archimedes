#pragma once

#include "placeholder.hpp"
#include "find.hpp"

namespace arch::ts {
template<class Ph, class... Ts>
struct ph_contains {
	static inline constexpr bool value = ph_find<true, Ph, Ts...>::value != -1;
};

template<class Ph, class Ph2>
struct ph_contains_other {
	static inline constexpr bool value = false;
};

template<class Ph, class... Ts>
struct ph_contains_other<Ph, ph<Ts...>> : ph_contains<Ph, Ts...> {};

template<class Ph, class... Ts>
struct ph_contains_any;

template<class Ph>
struct ph_contains_any<Ph> {
	static inline constexpr bool value = false;
};

template<class Ph, class T, class... Ts>
struct ph_contains_any<Ph, T, Ts...> {
	static inline constexpr bool value = ph_contains<Ph, T>::value or ph_contains_any<Ph, Ts...>::value;
};

template<class Ph, class Ph2>
struct ph_contains_any_from {
	static inline constexpr bool value = false;
};

template<class Ph, class T, class... Ts>
struct ph_contains_any_from<Ph, ph<T, Ts...>> : ph_contains_any<Ph, T, Ts...> {};
}