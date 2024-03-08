#pragma once

#include "placeholder.hpp"

namespace arch::ts {
template<class Ph, template<class Ph2> class O, size_t N>
struct ph_do_n_times {
	using type = Ph;
};

template<template<class Ph2> class O, class... Ts>
struct ph_do_n_times<ph<Ts...>, O, 0> {
	using type = ph<Ts...>;
};

template<template<class Ph2> class O, size_t N, class... Ts>
struct ph_do_n_times<ph<Ts...>, O, N> {
	using after_one = O<ph<Ts...>>::type;

	using type = ph_do_n_times<after_one, O, N - 1>::type;
};
}