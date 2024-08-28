#pragma once

#include "QMod.h"

namespace arch::math {

inline auto qmod(std::unsigned_integral auto value, std::unsigned_integral auto div) noexcept {
	return value & (div - (decltype(div))1);
}

template<std::unsigned_integral auto Div>
requires(std::has_single_bit(Div))
inline auto qmod(std::unsigned_integral auto value) noexcept {
	constexpr auto DivSubOne = Div - (decltype(Div))1;
	return value & DivSubOne;
}

} // namespace arch::math
