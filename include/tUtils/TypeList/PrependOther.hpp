#pragma once

#include "Cat.hpp"

namespace arch::tUtils::typeList {

/// @brief Prepends types from placeholders
/// @tparam Ph - placeholder type
/// @tparam Phs - placeholders to prepend
template<class Ph, class... Phs>
struct PhPrependOther {
	using type = Ph;
};

template<class... Ts, class... Phs>
struct PhPrependOther<_Ph<Ts...>, Phs...>: PhCat<true, Phs..., _Ph<Ts...>> {};

} // namespace arch::tUtils::typeList
