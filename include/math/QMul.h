#pragma once

#include <bit>
#include <concepts>
#include <type_traits>

namespace arch::math {

/// @brief Computes (value * mul), optimized for multiplier being power of two
/// @param value - value to multiply
/// @param mul - multiplier
inline auto qmul(std::unsigned_integral auto value, std::unsigned_integral auto mul) noexcept;

/// @brief Computes (value * Mul), optimized for multiplier being power of two
/// @tparam Mul - multiplier
/// @param value - value to multiply
template<std::unsigned_integral auto Mul>
requires(std::has_single_bit(Mul))
inline auto qmul(std::unsigned_integral auto value) noexcept;

/// @brief Computes (value * pow(2, mulPow)), optimized for multiplier being power of two
/// @param value - value to multiply
/// @param mulPow - log2(multiplier)
/// @see std::countr_zero
inline auto qmulPow(std::unsigned_integral auto value, std::unsigned_integral auto mulPow) noexcept;

/// @brief Computes (value * pow(2, MulPow)), optimized for multiplier being power of two
/// @tparam DivPow - log2(multiplier)
/// @param value - value to multiply
/// @see std::countr_zero
template<std::unsigned_integral auto MulPow>
inline auto qmulPow(std::unsigned_integral auto value) noexcept;

} // namespace arch::math

#include "QMul.hpp"
