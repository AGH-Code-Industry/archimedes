#pragma once

#include "Append.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeList {

/// @brief Reverses placeholder
/// @tparam Ph - placeholder type
template<class Ph>
struct PhReverse {
	using type = Ph;
};

template<class T, class... Ts>
struct PhReverse<_Ph<T, Ts...>> {
	using type = PhAppend<true, typename PhReverse<_Ph<Ts...>>::type, T>::type;
};

} // namespace arch::tUtils::typeList
