#pragma once

#include "cat.hpp"

namespace arch::ts {
template<class Ph, class... Phs>
struct ph_append_other {
	using type = Ph;
};
template<class... Ts, class... Phs>
struct ph_append_other<ph<Ts...>, Phs...> : ph_cat<true, ph<Ts...>, Phs...> {};

template<class Ph, class... Phs>
struct ph_prepend_other {
	using type = Ph;
};
template<class... Ts, class... Phs>
struct ph_prepend_other<ph<Ts...>, Phs...> : ph_cat<true, Phs..., ph<Ts...>> {};
}