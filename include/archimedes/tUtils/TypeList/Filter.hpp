#pragma once

#include "Cat.hpp"
#include "Placeholder.hpp"
#include "SubList.hpp"

namespace arch::tUtils::typeList {

template<class Ph, template<class> class Pred, size_t Pos>
struct PhFilter {
	using type = Ph;
};

template<class... Ts, template<class> class Pred, size_t Pos>
struct PhFilter<_Ph<Ts...>, Pred, Pos> {
private:

	using head = PhSubList<true, _Ph<Ts...>, 0, Pos>::type;
	using tail = PhSubList<true, _Ph<Ts...>, Pos, (size_t)-1>::type;

public:

	using type = PhFilter<tail, Pred, 0>::type;
};

template<class... Ts, template<class> class Pred>
struct PhFilter<_Ph<Ts...>, Pred, 0> {
	using type = PhCat<true, std::conditional_t<Pred<Ts>::value, _Ph<Ts>, _Ph<>>...>::type;
};

} // namespace arch::tUtils::typeList
