#pragma once

#include "Placeholder.hpp"

namespace arch::tUtils::typeString {
template<bool R, class Ph, class... Ts>
struct PhPrepend {
	using type = Ph;
};

template<class Ph, class... Ts>
struct PhPrepend<false, Ph, Ts...> {
	using type = typeStringRecursionEnd;
};

template<class... Ts1, class... Ts2>
struct PhPrepend<true, _Ph<Ts1...>, Ts2...> {
	using type = _Ph<Ts2..., Ts1...>;
};
} // namespace arch::tUtils::typeString
