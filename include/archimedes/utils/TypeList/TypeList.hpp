#pragma once

#include <concepts>

#include "TypeListFwd.h"

namespace arch::utils::details {

/// @brief Helper for static_asserts, disables false-positives
template<size_t>
inline constexpr bool alwaysFalse = false;

/// @brief Helper alias that converts size_t values to void* type
template<size_t>
using IndexedVoidPtr = void*;

/// @brief Helper struct with function fn, for converting first I given pointers to void*
/// @tparam <unnamed> - std::index_sequence
template<class>
struct TLGetFn;

template<size_t... Indexes>
struct TLGetFn<std::index_sequence<Indexes...>> {
	/// @brief Helper function, first I pointers are converted to void*, the I-th pointer gets returned
	/// @details Explanation:
	/// @details I == sizeof...(Indexes).
	/// @details IndexedVoidPtr<Indexes>... makes I first parameters of fn to be void*
	/// @details Head* is the I-th type, the one we're searching for
	/// @details ... is placeholder for the rest of the types
	/// @details Example:
	/// @details Get<2, int, float, char, void, double>
	/// @details uses make_index_sequence<2> == index_sequence<0, 1> => Indexes... == 0, 1;
	/// @details Indexes... expands to void*, void*
	/// @details Head* is a template param, awaiting to be matched
	/// @details fn was called as fn(int*, float*, char*, void*, double*),
	/// @details since first 2 pointers are void*, the next param is Head* and the rest is discarded:
	/// @details fn(int*, float*, char*, void*, double*) => fn(void*, void*, char*, ...),
	/// @details so Head* == char*
	/// @tparam Head - returned type
	/// @param ... - pointers to search
	template<class Head>
	static consteval Head* fn(IndexedVoidPtr<Indexes>..., Head*, ...) { // evil type-matching level hacking
		return nullptr;
	}
};

/// @brief Helper struct for obtaining I-th element in type pack
/// @detail Time complexity: 0(1) instead of recursive O(N)
/// @tparam Types... - types
/// @tparam I - index of type to get
/// @todo Use pack indexing instead of pointer-hacks once (or maybe 'if' XD) C++26 arrives
template<size_t I, class... Types>
struct TLGet {
	using type = std::remove_pointer_t<decltype(TLGetFn<std::make_index_sequence<I>>::fn(((Types*)nullptr)...))>;
};

/// @brief Nots given boolean UnaryTypeTrait
/// @tparam TypeTrait - type trait to not
template<template<class> class TypeTrait>
struct NotTrait {
	template<class T>
	using type = std::bool_constant<!TypeTrait<T>::value>;
};

/// @brief Helper struct, providing an optional alias to the first type from pack
/// @tparam T... - type pack
/// @tparam V - whether to extract first type
template<bool V, class... T>
struct SingleTypeAlias {};

// Specialization
template<class T, class... Ts>
struct SingleTypeAlias<true, T, Ts...> {
	using type = T;

	template<class T2>
	static consteval bool is(TypeList<T2> = {}) {
		return std::same_as<T, T2>;
	}

	template<auto Fn>
	static consteval auto apply() {
		return Fn(typelist<T>);
	}

	template<template<class> class Trait>
	static consteval auto apply() {
		return Trait<T>::value;
	}
};

} // namespace arch::utils::details
