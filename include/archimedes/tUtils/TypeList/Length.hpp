#pragma once

#include "Placeholder.hpp"

namespace arch::tUtils::typeList {

/// @brief Returns length of placeholder
/// @tparam Ph - placeholder type
template<class Ph>
struct PhLength {
	static inline constexpr size_t value = 0;
};

template<class... Ts>
struct PhLength<_Ph<Ts...>> {
	static inline constexpr size_t value = sizeof...(Ts);
};

/// @brief Checks if placeholder is empty
/// @tparam Ph - placeholder type
template<class Ph>
struct PhEmpty {
	static inline constexpr bool value = PhLength<Ph>::value != 0;
};

} // namespace arch::tUtils::typeList
