#pragma once

#include "pop.hpp"

namespace arch::ts {
template<bool R, class Ph, size_t N>
struct ph_pop_n_front {
	using type = Ph;
};

template<class Ph, size_t N>
struct ph_pop_n_front<false, Ph, N> {
	using type = type_string_recursion_end;
};

template<size_t N>
struct ph_pop_n_front<true, ph<>, N> {
	using type = ph<>;
};

template<class T, class... Ts>
struct ph_pop_n_front<true, ph<T, Ts...>, 0> {
	using type = ph<T, Ts...>;
};

template<class... Ts, size_t N>
struct ph_pop_n_front<true, ph<Ts...>, N> {
	using popped = ph_pop_front<true, ph<Ts...>>::type;

	using type = ph_pop_n_front<true, popped, N - 1>::type;
};

template<bool R, class Ph, size_t N>
struct ph_pop_n_back {
	using type = Ph;
};

template<class Ph, size_t N>
struct ph_pop_n_back<false, Ph, N> {
	using type = type_string_recursion_end;
};

template<size_t N>
struct ph_pop_n_back<true, ph<>, N> {
	using type = ph<>;
};

template<class T, class... Ts>
struct ph_pop_n_back<true, ph<T, Ts...>, 0> {
	using type = ph<T, Ts...>;
};

template<class... Ts, size_t N>
struct ph_pop_n_back<true, ph<Ts...>, N> {
	using popped = ph_pop_back<true, ph<Ts...>>::type;

	using type = ph_pop_n_back<true, popped, N - 1>::type;
};
}