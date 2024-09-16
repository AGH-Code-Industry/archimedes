#pragma once

#include <bit>

#include "QMul.h"

namespace arch::math {

inline auto qmul(std::unsigned_integral auto value, std::unsigned_integral auto mul) noexcept {
	return value << std::countr_zero(mul);
}

template<std::unsigned_integral auto Mul>
requires(std::has_single_bit(Mul))
inline auto qmul(std::unsigned_integral auto value) noexcept {
	constexpr decltype(Mul) mul = std::countr_zero(Mul);
	return value << mul;
}

inline auto qmulPow(std::unsigned_integral auto value, std::unsigned_integral auto mulPow) noexcept {
	return value << mulPow;
}

template<std::unsigned_integral auto MulPow>
inline auto qmulPow(std::unsigned_integral auto value) noexcept {
	return value << MulPow;
}

} // namespace arch::math
