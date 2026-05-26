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
	using type = typename TLGet<I - 1, Tail...>::type;
};

template<class Head, class... Tail>
struct TLGet<0, Head, Tail...> {
	using type = Head;
};

template<bool V, class T>
struct SingleFilter {
	using type = TypeList<>;
};

template<class T>
struct SingleFilter<true, T> {
	using type = TypeList<T>;
};

template<class...>
struct TLCat {
	using type = TypeList<>;
};

template<class... Types>
struct TLCat<TypeList<Types...>> {
	using type = TypeList<Types...>;
};

template<class... Types, class... Types2, class... Rest>
struct TLCat<TypeList<Types...>, TypeList<Types2...>, Rest...> {
	using type = typename TLCat<TypeList<Types..., Types2...>, Rest...>::type;
};

template<template<class> class TypeTrait>
struct NotTrait {
	template<class T>
	using type = std::bool_constant<!TypeTrait<T>::value>;
};

template<bool V, class... T>
struct SingleTypeAlias {};

template<class T, class... Ts>
struct SingleTypeAlias<true, T, Ts...> {
	using type = T;
};

} // namespace arch::utils::details
