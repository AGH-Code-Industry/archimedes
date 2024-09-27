#pragma once

#include "Cat.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeString {
template<bool R, class Ph>
struct PhPopFront {
	using type = Ph;
};

template<class Ph>
struct PhPopFront<false, Ph> {
	using type = typeStringRecursionEnd;
};

template<class T, class... Ts>
struct PhPopFront<true, _Ph<T, Ts...>> {
	using type = _Ph<Ts...>;
};

template<bool R, class Ph>
struct PhPopBack {
	using type = Ph;
};

template<class Ph>
struct PhPopBack<false, Ph> {
	using type = typeStringRecursionEnd;
};

template<class T, class... Ts>
struct PhPopBack<true, _Ph<T, Ts...>> {
	using popped = PhPopBack<true, _Ph<Ts...>>::type;

	using type = PhCat<true, _Ph<T>, popped>::type;
};

template<class T>
struct PhPopBack<true, _Ph<T>> {
	using type = _Ph<>;
};
} // namespace arch::tUtils::typeString
