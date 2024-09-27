#pragma once

#include "EraseIf.hpp"
#include "Pop.hpp"

namespace arch::tUtils::typeString {
template<class Ph>
struct PhDistinct {
	using type = Ph;
};

template<class T, class... Ts>
struct PhDistinct<_Ph<T, Ts...>> {
	using head = _Ph<T>;
	using erasedT = typename PhEraseT<_Ph<Ts...>, T, 0>::type;
	using tail = PhDistinct<erasedT>::type;

	using type = PhCat<true, head, tail>::type;
};
} // namespace arch::tUtils::typeString
