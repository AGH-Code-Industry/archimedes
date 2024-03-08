#pragma once

#include "placeholder.hpp"
#include "substr.hpp"

namespace arch::ts {
template<class Ph, size_t I>
struct ph_get {
	using type = Ph;
};

template<class T, class... Ts>
struct ph_get<ph<T, Ts...>, 0> {
	using type = T;
};

template<class... Ts, size_t I>
struct ph_get<ph<Ts...>, I> {
	static inline constexpr bool out_of_bounds = (I >= sizeof...(Ts));

	using type = cond_t<out_of_bounds, type_string_none_type, typename ph_get<typename ph_substr<not out_of_bounds, ph<Ts...>, I, 1>::type, 0>::type>;
};
}