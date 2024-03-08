#pragma once

#include "placeholder.hpp"
#include "append.hpp"

namespace arch::ts {
template<class Ph>
struct ph_reverse {
	using type = Ph;
};

template<class T, class... Ts>
struct ph_reverse<ph<T, Ts...>> {
	using type = ph_append<true, typename ph_reverse<ph<Ts...>>::type, T>::type;
};
}