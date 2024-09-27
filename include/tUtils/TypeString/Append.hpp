#pragma once

#include "Placeholder.hpp"

namespace arch::tUtils::typeString {
template<bool R, class Ph, class... Ts>
struct PhAppend {
	using type = Ph;
};

template<class Ph, class... Ts>
struct PhAppend<false, Ph, Ts...> {
	using type = typeStringRecursionEnd;
};

template<class... Ts1, class... Ts2>
struct PhAppend<true, _Ph<Ts1...>, Ts2...> {
	using type = _Ph<Ts1..., Ts2...>;
};
} // namespace arch::tUtils::typeString
