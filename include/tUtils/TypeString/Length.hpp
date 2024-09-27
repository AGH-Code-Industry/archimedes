#pragma once

#include "Placeholder.hpp"

namespace arch::tUtils::typeString {
template<class Ph>
struct PhLength {
	static inline constexpr size_t value = 0;
};

template<class... Ts>
struct PhLength<_Ph<Ts...>> {
	static inline constexpr size_t value = sizeof...(Ts);
};

template<class Ph>
struct PhEmpty {
	static inline constexpr bool value = PhLength<Ph>::value != 0;
};
} // namespace arch::tUtils::typeString
