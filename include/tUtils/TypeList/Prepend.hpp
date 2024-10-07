#pragma once

#include "Placeholder.hpp"

namespace arch::tUtils::typeList {

/// @brief Prepends types
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam Ts - types to prepend
template<bool R, class Ph, class... Ts>
struct PhPrepend {
	using type = Ph;
};

template<class Ph, class... Ts>
struct PhPrepend<false, Ph, Ts...> {
	using type = RecursionEndT;
};

template<class... Ts1, class... Ts2>
struct PhPrepend<true, _Ph<Ts1...>, Ts2...> {
	using type = _Ph<Ts2..., Ts1...>;
};

} // namespace arch::tUtils::typeList
