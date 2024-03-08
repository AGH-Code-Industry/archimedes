#pragma once

#include "substr.hpp"
#include "append.hpp"
#include "cat.hpp"

namespace arch::ts {
template<class Ph, size_t Pos, class... Ts>
struct ph_insert {
	using type = Ph;
};

template<size_t Pos, class... Ts, class... Ts2>
struct ph_insert<ph<Ts...>, Pos, Ts2...> {
	using first = ph_substr<true, ph<Ts...>, 0, Pos>::type;
	using second = ph_substr<true, ph<Ts...>, Pos, -1>::type;

	using type = ph_cat<true, first, ph<Ts2...>, second>::type;
};

template<class Ph, size_t Pos, class... Phs>
struct ph_insert_other {
	using type = Ph;
};

template<class... Ts, size_t Pos, class... Ts2>
struct ph_insert_other<ph<Ts...>, Pos, ph<Ts2...>> : ph_insert<ph<Ts...>, Pos, Ts2...> {};

template<class... Ts, size_t Pos, class... Phs>
struct ph_insert_other<ph<Ts...>, Pos, Phs...> : ph_insert_other<ph<Ts...>, Pos, typename ph_cat<true, Phs...>::type> {};
}