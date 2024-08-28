#pragma once

#include <bit>
#include <concepts>
#include <type_traits>

namespace arch::math {

/// @brief Computes (value / div), optimized for divisor being power of two
/// @param value - value to divide
/// @param div - divisor
inline auto qdiv(std::unsigned_integral auto value, std::unsigned_integral auto div) noexcept;

/// @brief Computes (value / Div), optimized for divisor being power of two
/// @tparam Div - divisor
/// @param value - value to divide
template<std::unsigned_integral auto Div>
requires(std::has_single_bit(Div))
inline auto qdiv(std::unsigned_integral auto value) noexcept;

/// @brief Computes (value / pow(2, divPow)), optimized for divisor being power of two
/// @param value - value to divide
/// @param divPow - log2(divisor)
/// @see std::countr_zero
inline auto qdivPow(std::unsigned_integral auto value, std::unsigned_integral auto divPow) noexcept;

/// @brief Computes (value / pow(2, DivPow)), optimized for divisor being power of two
/// @tparam DivPow - log2(divisor)
/// @param value - value to divide
/// @see std::countr_zero
template<std::unsigned_integral auto DivPow>
inline auto qdivPow(std::unsigned_integral auto value) noexcept;

} // namespace arch::math

#include "QDiv.hpp"
