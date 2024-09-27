#pragma once

#include "Pop.hpp"

namespace arch::tUtils::typeString {
template<bool R, class Ph, size_t N>
struct PhPopNFront {
	using type = Ph;
};

template<class Ph, size_t N>
struct PhPopNFront<false, Ph, N> {
	using type = typeStringRecursionEnd;
};

template<size_t N>
struct PhPopNFront<true, _Ph<>, N> {
	using type = _Ph<>;
};

template<class T, class... Ts>
struct PhPopNFront<true, _Ph<T, Ts...>, 0> {
	using type = _Ph<T, Ts...>;
};

template<class... Ts, size_t N>
struct PhPopNFront<true, _Ph<Ts...>, N> {
	using popped = PhPopFront<true, _Ph<Ts...>>::type;

	using type = PhPopNFront<true, popped, N - 1>::type;
};

template<bool R, class Ph, size_t N>
struct PhPopNBack {
	using type = Ph;
};

template<class Ph, size_t N>
struct PhPopNBack<false, Ph, N> {
	using type = typeStringRecursionEnd;
};

template<size_t N>
struct PhPopNBack<true, _Ph<>, N> {
	using type = _Ph<>;
};

template<class T, class... Ts>
struct PhPopNBack<true, _Ph<T, Ts...>, 0> {
	using type = _Ph<T, Ts...>;
};

template<class... Ts, size_t N>
struct PhPopNBack<true, _Ph<Ts...>, N> {
	using popped = PhPopBack<true, _Ph<Ts...>>::type;

	using type = PhPopNBack<true, popped, N - 1>::type;
};
} // namespace arch::tUtils::typeString
