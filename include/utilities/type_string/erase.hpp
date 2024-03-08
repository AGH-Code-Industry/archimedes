#pragma once

#include "placeholder.hpp"
#include "cat.hpp"
#include "substr.hpp"

namespace arch::ts {
template<class Ph, size_t Pos, size_t Count>
struct ph_erase {
	using type = Ph;
};

template<class... Ts, size_t Pos, size_t Count>
struct ph_erase<ph<Ts...>, Pos, Count> {
	using first = ph_substr<true, ph<Ts...>, 0, Pos>::type;
	using second = ph_substr<true, ph<Ts...>, Pos + Count, -1>::type;
	
	using type = ph_cat<true, first, second>::type;
};
}