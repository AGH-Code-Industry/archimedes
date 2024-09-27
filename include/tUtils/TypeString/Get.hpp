#pragma once

#include "Placeholder.hpp"
#include "Substr.hpp"

namespace arch::tUtils::typeString {
template<class Ph, size_t I>
struct PhGet {
	using type = Ph;
};

template<class T, class... Ts>
struct PhGet<_Ph<T, Ts...>, 0> {
	using type = T;
};

template<class... Ts, size_t I>
struct PhGet<_Ph<Ts...>, I> {
	static inline constexpr bool outOfBounds = (I >= sizeof...(Ts));

	using type = CondT<
		outOfBounds,
		typeStringNoneType,
		typename PhGet<typename PhSubstr<not outOfBounds, _Ph<Ts...>, I, 1>::type, 0>::type>;
};
} // namespace arch::tUtils::typeString
