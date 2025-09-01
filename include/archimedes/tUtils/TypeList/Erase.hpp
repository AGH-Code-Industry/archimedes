#pragma once

#include "Cat.hpp"
#include "Placeholder.hpp"
#include "SubList.hpp"

namespace arch::tUtils::typeList {

/// @brief Erases types
/// @tparam Ph - placeholder type
/// @tparam Pos - where to start erasing
/// @tparam Count - count of types to erase
template<class Ph, size_t Pos, size_t Count>
struct PhErase {
	using type = Ph;
};

template<class... Ts, size_t Pos, size_t Count>
struct PhErase<_Ph<Ts...>, Pos, Count> {
private:

	using first = PhSubList<true, _Ph<Ts...>, 0, Pos>::type;
	using second = PhSubList<true, _Ph<Ts...>, Pos + Count, (size_t)-1>::type;

public:

	using type = PhCat<true, first, second>::type;
};

} // namespace arch::tUtils::typeList
