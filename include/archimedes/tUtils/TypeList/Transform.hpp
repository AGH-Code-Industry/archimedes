#pragma once

#include "Cat.hpp"
#include "Placeholder.hpp"
#include "SubList.hpp"

namespace arch::tUtils::typeList {

template<class Ph, template<class> class Fn, size_t Pos>
struct PhTransform {
	using type = Ph;
};

template<class... Ts, template<class> class Fn, size_t Pos>
struct PhTransform<_Ph<Ts...>, Fn, Pos> {
private:

	using head = PhSubList<true, _Ph<Ts...>, 0, Pos>::type;
	using tail = PhSubList<true, _Ph<Ts...>, Pos, (size_t)-1>::type;

public:

	using type = PhCat<true, head, typename PhTransform<tail, Fn, 0>::type>::type;
};

template<class... Ts, template<class> class Fn>
struct PhTransform<_Ph<Ts...>, Fn, 0> {
	using type = _Ph<typename Fn<Ts>::type...>;
};

} // namespace arch::tUtils::typeList
