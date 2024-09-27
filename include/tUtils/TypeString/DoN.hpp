#pragma once

#include "Placeholder.hpp"

namespace arch::tUtils::typeString {
template<class Ph, template<class Ph2> class O, size_t N>
struct PhDoN {
	using type = Ph;
};

template<template<class Ph2> class O, class... Ts>
struct PhDoN<_Ph<Ts...>, O, 0> {
	using type = _Ph<Ts...>;
};

template<template<class Ph2> class O, size_t N, class... Ts>
struct PhDoN<_Ph<Ts...>, O, N> {
	using after_one = O<_Ph<Ts...>>::type;

	using type = PhDoN<after_one, O, N - 1>::type;
};
} // namespace arch::tUtils::typeString
