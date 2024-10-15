#pragma once

#include "Placeholder.hpp"
#include "SubList.hpp"

namespace arch::tUtils::typeList {

/// @brief Gets type at given index
/// @tparam Ph - placeholder type
/// @tparam I - index of type to get
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

	using type = std::conditional_t<
		outOfBounds,
		NoneT,
		typename PhGet<typename PhSubList<!outOfBounds, _Ph<Ts...>, I, 1>::type, 0>::type>;
};

} // namespace arch::tUtils::typeList
