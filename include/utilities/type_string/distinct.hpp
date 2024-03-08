#pragma once

#include "erase_if.hpp"
#include "pop.hpp"

namespace arch::ts {
template<class Ph>
struct ph_distinct {
	using type = Ph;
};

template<class T, class... Ts>
struct ph_distinct<ph<T, Ts...>> {
	using head = ph<T>;
	using erased_t = ph_erase_t<ph<Ts...>, T>::type;
	using tail = ph_distinct<erased_t>::type;

	using type = ph_cat<true, head, tail>::type;
};
}