#pragma once

#include "Cat.hpp"

namespace arch::tUtils::typeString {
template<class Ph, class... Phs>
struct PhPrependOther {
	using type = Ph;
};

template<class... Ts, class... Phs>
struct PhPrependOther<_Ph<Ts...>, Phs...>: PhCat<true, Phs..., _Ph<Ts...>> {};
} // namespace arch::tUtils::typeString
