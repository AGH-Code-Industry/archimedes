#pragma once

#include <bit>
#include <concepts>
#include <type_traits>

namespace arch::math {

/// @brief Computes (value % div), optimized for divisor being power of two
/// @param value - value to divide
/// @param div - divisor
inline auto qmod(std::unsigned_integral auto value, std::unsigned_integral auto div) noexcept;

/// @brief Computes (value % Div), optimized for divisor being power of two
/// @tparam Div - divisor
/// @param value - value to divide
template<std::unsigned_integral auto Div>
requires(std::has_single_bit(Div))
inline auto qmod(std::unsigned_integral auto value) noexcept;

} // namespace arch::math

#include "QMod.hpp"
