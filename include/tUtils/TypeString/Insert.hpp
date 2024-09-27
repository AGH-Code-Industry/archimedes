#pragma once

#include "Append.hpp"
#include "Cat.hpp"
#include "Substr.hpp"

namespace arch::tUtils::typeString {
template<class Ph, size_t Pos, class... Ts>
struct PhInsert {
	using type = Ph;
};

template<size_t Pos, class... Ts, class... Ts2>
struct PhInsert<_Ph<Ts...>, Pos, Ts2...> {
	using first = PhSubstr<true, _Ph<Ts...>, 0, Pos>::type;
	using second = PhSubstr<true, _Ph<Ts...>, Pos, (size_t)-1>::type;

	using type = PhCat<true, first, _Ph<Ts2...>, second>::type;
};

template<class Ph, size_t Pos, class... Phs>
struct PhInsertFrom {
	using type = Ph;
};

template<class... Ts, size_t Pos, class... Ts2>
struct PhInsertFrom<_Ph<Ts...>, Pos, _Ph<Ts2...>>: PhInsert<_Ph<Ts...>, Pos, Ts2...> {};

template<class... Ts, size_t Pos, class... Phs>
struct PhInsertFrom<_Ph<Ts...>, Pos, Phs...>: PhInsertFrom<_Ph<Ts...>, Pos, typename PhCat<true, Phs...>::type> {};
} // namespace arch::tUtils::typeString
