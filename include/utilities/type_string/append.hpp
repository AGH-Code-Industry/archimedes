#pragma once

#include "placeholder.hpp"

namespace arch::ts {
template<bool R, class Ph, class... Ts>
struct ph_append {
	using type = Ph;
};

template<class Ph, class... Ts>
struct ph_append<false, Ph, Ts...> {
	using type = type_string_recursion_end;
};

template<class... Ts1, class... Ts2>
struct ph_append<true, ph<Ts1...>, Ts2...> {
	using type = ph<Ts1..., Ts2...>;
};
}