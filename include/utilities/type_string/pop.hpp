#pragma once

#include "placeholder.hpp"
#include "cat.hpp"

namespace arch::ts {
template<bool R, class Ph>
struct ph_pop_front {
	using type = Ph;
};

template<class Ph>
struct ph_pop_front<false, Ph> {
	using type = type_string_recursion_end;
};

template<class T, class... Ts>
struct ph_pop_front<true, ph<T, Ts...>> {
	using type = ph<Ts...>;
};

template<bool R, class Ph>
struct ph_pop_back {
	using type = Ph;
};

template<class Ph>
struct ph_pop_back<false, Ph> {
	using type = type_string_recursion_end;
};

template<class T, class... Ts>
struct ph_pop_back<true, ph<T, Ts...>> {
	using popped = ph_pop_back<true, ph<Ts...>>::type;

	using type = ph_cat<true, ph<T>, popped>::type;
};

template<class T>
struct ph_pop_back<true, ph<T>> {
	using type = ph<>;
};
}