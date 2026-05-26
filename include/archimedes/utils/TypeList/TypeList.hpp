#pragma once

#include <concepts>

#include "TypeListFwd.h"

namespace arch::utils::details {

template<size_t>
inline constexpr bool alwaysFalse = false;

template<size_t I, class... Types>
struct TLGet;

template<size_t I, class Head, class... Tail>
struct TLGet<I, Head, Tail...> {
	using inner = TLGet<I - 1, Tail...>;
	using type = typename TLGet<I - 1, Tail...>::type;
};

template<class Head, class... Tail>
struct TLGet<0, Head, Tail...> {
	using type = Head;
};

// template<size_t N, class TL>
// struct PopFrontImpl {
//	using type = TypeList<Types...>;
// };
//
// template<size_t N, class Head, class... Tail>
// requires(N != 0)
// struct PopFrontImpl<N, Head, Tail...> {
//	using type = typename PopFrontImpl<N - 1, Tail...>::type;
// };
//
// template<class... Types>
// struct FrontImpl {
//	using type = TypeList<Types...>;
// };
//
// template<class Head, class... Tail>
// struct FrontImpl<Head, Tail...> {
//	using type = TypeList<Head>;
// };

template<auto Pred>
struct NotPred {
	constexpr bool operator()(auto tl) const { return !Pred(tl); }
};

template<template<class> class TypeTrait>
struct NotTrait {
	template<class T>
	using type = std::bool_constant<!TypeTrait<T>::value>;
};

template<auto Pred>
inline constexpr NotPred<Pred> notPred{};

template<bool V, class... T>
struct SingleTypeAlias {};

template<class T, class... Ts>
struct SingleTypeAlias<true, T, Ts...> {
	using type = T;
};

} // namespace arch::utils::details
