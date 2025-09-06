#pragma once

#include "Cat.hpp"

namespace arch::tUtils::typeList {

/// @brief Appends types from placeholders
/// @tparam Ph - placeholder type
/// @tparam Phs - placeholders to append
template<class Ph, class... Phs>
struct PhAppendOther {
	using type = Ph;
};

template<class... Ts, class... Phs>
struct PhAppendOther<_Ph<Ts...>, Phs...>: PhCat<true, _Ph<Ts...>, Phs...> {};

} // namespace arch::tUtils::typeList
