#pragma once

#include "EraseIf.hpp"
#include "Pop.hpp"

namespace arch::tUtils::typeList {

/// @brief Erases equivalent types, leaving only first occurence
/// @tparam Ph - placeholder type
template<class Ph>
struct PhDistinct {
	using type = Ph;
};

template<class T, class... Ts>
struct PhDistinct<_Ph<T, Ts...>> {
private:

	using head = _Ph<T>;
	using erasedT = typename PhEraseT<_Ph<Ts...>, T, 0>::type;
	using tail = PhDistinct<erasedT>::type;

public:

	using type = PhCat<true, head, tail>::type;
};

} // namespace arch::tUtils::typeList
