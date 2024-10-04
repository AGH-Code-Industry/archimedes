#pragma once

#include "Pop.hpp"

namespace arch::tUtils::typeList {

/// @brief Pops N types from front
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam N - types to pop
template<bool R, class Ph, size_t N>
struct PhPopNFront {
	using type = Ph;
};

template<class Ph, size_t N>
struct PhPopNFront<false, Ph, N> {
	using type = RecursionEndT;
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
private:

	using popped = PhPopFront<true, _Ph<Ts...>>::type;

public:

	using type = PhPopNFront<true, popped, N - 1>::type;
};

/// @brief Pops N types from back
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam N - types to pop
template<bool R, class Ph, size_t N>
struct PhPopNBack {
	using type = Ph;
};

template<class Ph, size_t N>
struct PhPopNBack<false, Ph, N> {
	using type = RecursionEndT;
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
private:

	using popped = PhPopBack<true, _Ph<Ts...>>::type;

public:

	using type = PhPopNBack<true, popped, N - 1>::type;
};

} // namespace arch::tUtils::typeList
