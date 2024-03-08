#pragma once

#include "placeholder.hpp"

namespace arch::ts {
template<class Ph>
struct ph_length {
	static inline constexpr size_t value = 0;
};

template<class... Ts>
struct ph_length<ph<Ts...>> {
	static inline constexpr size_t value = sizeof...(Ts);
};

template<class Ph>
struct ph_empty {
	static inline constexpr bool value = ph_length<Ph>::value != 0;
};
}