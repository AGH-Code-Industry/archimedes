#pragma once

#include "placeholder.hpp"
#include "append.hpp"

namespace arch::ts {
template<bool R, class Ph, class... Phs>
struct ph_cat {
	using type = Ph;
};

template<class Ph, class... Phs>
struct ph_cat<false, Ph, Phs...> {
	using type = type_string_recursion_end;
};

template<class Ph>
struct ph_cat<true, Ph> {
	using type = Ph;
};

template<class Ph, class... Phs>
struct ph_cat<true, Ph, Phs...> : ph_cat<true, Phs...> {};

template<class... Ts, class Ph2, class... Phs>
struct ph_cat<true, ph<Ts...>, Ph2, Phs...> : ph_cat<true, ph<Ts...>, Phs...> {};

template<class... Ts, class... Ts2, class... Phs>
struct ph_cat<true, ph<Ts...>, ph<Ts2...>, Phs...> : ph_cat<true, ph<Ts..., Ts2...>, Phs...> {};
}