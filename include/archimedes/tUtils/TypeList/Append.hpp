#pragma once

#include "Placeholder.hpp"

namespace arch::tUtils::typeList {

/// @brief Appends types
/// @tparam R - Whether to continue recursion
/// @tparam Ph - Placeholder type
/// @tparam Ts - Types to append
template<bool R, class Ph, class... Ts>
struct PhAppend {
	using type = Ph;
};

template<class Ph, class... Ts>
struct PhAppend<false, Ph, Ts...> {
	using type = RecursionEndT;
};

template<class... Ts1, class... Ts2>
struct PhAppend<true, _Ph<Ts1...>, Ts2...> {
	using type = _Ph<Ts1..., Ts2...>;
};

} // namespace arch::tUtils::typeList
