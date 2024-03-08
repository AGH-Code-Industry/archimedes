#pragma once

#include "placeholder.hpp"
#include "do_n_times.hpp"
#include "pop_n.hpp"
#include "iterator.hpp"

namespace arch::ts {
template<bool R, class Ph, size_t Pos, size_t Count>
struct ph_substr {
	using type = Ph;
};

template<class Ph, size_t Pos, size_t Count>
struct ph_substr<false, Ph, Pos, Count> {
	using type = type_string_recursion_end;
};

template<class... Ts, size_t Pos, size_t Count>
struct ph_substr<true, ph<Ts...>, Pos, Count> {
	using headless = ph_pop_n_front<true, ph<Ts...>, Pos>::type;
	static inline constexpr size_t tail_length = (ph_length<headless>::value > Count ? ph_length<headless>::value - Count : 0);
	
	using type = ph_pop_n_back<true, headless, tail_length>::type;
};

using test = ph_substr<true, ph<float, int, char, void, double>, 1, 3>::type;
}