#pragma once

#include "placeholder.hpp"

namespace arch::ts {
template<bool R, class Ph, class... Ts>
struct ph_prepend {
	using type = Ph;
};

template<class Ph, class... Ts>
struct ph_prepend<false, Ph, Ts...> {
	using type = type_string_recursion_end;
};

template<class... Ts1, class... Ts2>
struct ph_prepend<true, ph<Ts1...>, Ts2...> {
	using type = ph<Ts2..., Ts1...>;
};
}