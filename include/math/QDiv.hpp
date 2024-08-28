#pragma once

#include <bit>

#include "QDiv.h"

namespace arch::math {

inline auto qdiv(std::unsigned_integral auto value, std::unsigned_integral auto div) noexcept {
	return value >> std::countr_zero(div);
}

template<std::unsigned_integral auto Div>
requires(std::has_single_bit(Div))
inline auto qdiv(std::unsigned_integral auto value) noexcept {
	constexpr decltype(Div) div = std::countr_zero(Div);
	return value >> div;
}

inline auto qdivPow(std::unsigned_integral auto value, std::unsigned_integral auto divPow) noexcept {
	return value >> divPow;
}

template<std::unsigned_integral auto DivPow>
inline auto qdivPow(std::unsigned_integral auto value) noexcept {
	return value >> DivPow;
}

} // namespace arch::math
