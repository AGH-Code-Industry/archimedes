#pragma once

#include "Cat.hpp"
#include "Placeholder.hpp"
#include "Substr.hpp"

namespace arch::tUtils::typeString {
template<class Ph, size_t Pos, size_t Count>
struct PhErase {
	using type = Ph;
};

template<class... Ts, size_t Pos, size_t Count>
struct PhErase<_Ph<Ts...>, Pos, Count> {
	using first = PhSubstr<true, _Ph<Ts...>, 0, Pos>::type;
	using second = PhSubstr<true, _Ph<Ts...>, Pos + Count, (size_t)-1>::type;

	using type = PhCat<true, first, second>::type;
};
} // namespace arch::tUtils::typeString
