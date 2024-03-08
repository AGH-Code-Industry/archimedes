#pragma once

#include "placeholder.hpp"
#include "iterator.hpp"
#include "do_n_times.hpp"
#include "find.hpp"
#include "rfind.hpp"
#include "contains.hpp"
#include "length.hpp"
#include "get.hpp"
#include "get_i.hpp"
#include "cat.hpp"
#include "substr.hpp"
#include "insert.hpp"
#include "erase.hpp"
#include "erase_if.hpp"
#include "front_back.hpp"
#include "pop.hpp"
#include "pop_n.hpp"
#include "append.hpp"
#include "prepend.hpp"
#include "append_prepend_other.hpp"
#include "replace.hpp"
#include "starts_ends_with.hpp"
#include "reverse.hpp"
#include "distinct.hpp"

namespace arch {
namespace ts {
template<class... Ts>
struct type_string;

template<class Ph>
struct to_ts {
	using type = Ph;
};

template<class... Ts>
struct to_ts<ph<Ts...>> {
	using type = type_string<Ts...>;
};

template<class TS>
struct to_ph {
	using type = TS;
};

template<class... Ts>
struct to_ph<type_string<Ts...>> {
	using type = ph<Ts...>;
};

template<class... Ts>
struct type_string {
private:
	template<class... Ts2>
	friend struct type_string;

	using _ph_t = ph<Ts...>;

public:
	template<class... Ts>
	using append = typename to_ts<typename ph_append<true, _ph_t, Ts...>::type>::type;
	template<class... TS>
	using append_other = typename to_ts<typename ph_append_other<_ph_t, typename TS::_ph_t...>::type>::type;

	template<class... Ts>
	using prepend = typename to_ts<typename ph_prepend<true, _ph_t, Ts...>::type>::type;
	template<class... TS>
	using prepend_other = typename to_ts<typename ph_prepend_other<_ph_t, typename TS::_ph_t...>::type>::type;

	template<class... TS>
	using cat = typename to_ts<typename ph_cat<true, _ph_t, typename TS::_ph_t...>::type>::type;

	template<class... Ts>
	static inline constexpr bool contains = ph_contains<_ph_t, Ts...>::value;
	template<class TS>
	static inline constexpr bool contains_other = ph_contains_other<_ph_t, typename TS::_ph_t>::value;
	template<class... Ts>
	static inline constexpr bool contains_any = ph_contains_any<_ph_t, Ts...>::value;
	template<class TS>
	static inline constexpr bool contains_any_from = ph_contains_any_from<_ph_t, typename TS::_ph_t>::value;

	using distinct = typename to_ts<typename ph_distinct<_ph_t>::type>::type;

	template<size_t Pos = 0, size_t Count = 1>
	using erase = typename to_ts<typename ph_erase<_ph_t, Pos, Count>::type>::type;
	template<class T, size_t Pos = 0>
	using erase_t = typename to_ts<typename ph_erase_t<_ph_t, T, Pos>::type>::type;
	template<template<class T> class Pred, size_t Pos = 0>
	using erase_if = typename to_ts<typename ph_erase_if<_ph_t, Pred, Pos>::type>::type;

	template<size_t Pos, class... Ts>
	static inline constexpr size_t find_at = ph_find_at<true, _ph_t, Pos, Ts...>::value;
	template<class... Ts>
	static inline constexpr size_t find = ph_find<true, _ph_t, Ts...>::value;
	template<class TS, size_t Pos = 0>
	static inline constexpr size_t find_other = ph_find_other_at<true, _ph_t, Pos, typename TS::_ph_t>::value;

	template<size_t Pos, class... Ts>
	static inline constexpr size_t find_any_at = ph_find_any_at<true, _ph_t, Pos, Ts...>::value;
	template<class... Ts>
	static inline constexpr size_t find_any = ph_find_any<true, _ph_t, Ts...>::value;
	template<class TS, size_t Pos = 0>
	static inline constexpr size_t find_any_from = ph_find_any_from_at<true, _ph_t, Pos, typename TS::_ph_t>::value;

	template<size_t Pos, class... Ts>
	static inline constexpr size_t rfind_at = ph_rfind_at<true, _ph_t, Pos, Ts...>::value;
	template<class... Ts>
	static inline constexpr size_t rfind = ph_rfind<true, _ph_t, Ts...>::value;
	template<class TS, size_t Pos = 0>
	static inline constexpr size_t rfind_other = ph_rfind_other_at<true, _ph_t, Pos, typename TS::_ph_t>::value;

	template<size_t Pos, class... Ts>
	static inline constexpr size_t rfind_any_at = ph_rfind_any_at<true, _ph_t, Pos, Ts...>::value;
	template<class... Ts>
	static inline constexpr size_t rfind_any = ph_rfind_any<true, _ph_t, Ts...>::value;
	template<class TS, size_t Pos = 0>
	static inline constexpr size_t rfind_any_from = ph_rfind_any_from_at<true, _ph_t, Pos, typename TS::_ph_t>::value;

	using front = typename ph_front<_ph_t>::type;
	using back = typename ph_back<_ph_t>::type;

	template<size_t Pos, class... Ts>
	using insert = typename to_ts<typename ph_insert<_ph_t, Pos, Ts...>::type>::type;
	template<size_t Pos, class... TS>
	using insert_other = typename to_ts<typename ph_insert_other<_ph_t, Pos, typename TS::_ph_t...>::type>::type;

	static inline constexpr size_t length = sizeof...(Ts);

	using pop_front = typename to_ts<typename ph_pop_front<true, _ph_t>::type>::type;
	using pop_back = typename to_ts<typename ph_pop_back<true, _ph_t>::type>::type;
	template<size_t N>
	using pop_n_front = typename to_ts<typename ph_pop_n_front<true, _ph_t, N>::type>::type;
	template<size_t N>
	using pop_n_back = typename to_ts<typename ph_pop_n_back<true, _ph_t, N>::type>::type;

	using reverse = typename to_ts<typename ph_reverse<_ph_t>::type>::type;

	template<class... Ts>
	static inline constexpr bool starts_with = ph_starts_with<_ph_t, Ts...>::value;
	template<class TS>
	static inline constexpr bool starts_with_other = ph_starts_with_other<_ph_t, TS::_ph_t>::value;
	template<class... Ts>
	static inline constexpr bool ends_with = ph_ends_with<_ph_t, Ts...>::value;
	template<class TS>
	static inline constexpr bool ends_with_other = ph_ends_with_other<_ph_t, TS::_ph_t>::value;

	template<size_t Pos = 0, size_t Count = -1>
	using substr = typename to_ts<typename ph_substr<true, _ph_t, Pos, Count>::type>::type;

	using id = type_string<Ts...>;
};
}

using ts::type_string;

template<class T>
struct is_leq_4_bytes {
	static inline constexpr bool value = sizeof(T) <= 4;
};

template<>
struct is_leq_4_bytes<void> {
	static inline constexpr bool value = false;
};
}