#pragma once

#include <concepts>

namespace arch::utils {

template<class T, T v>
class ConstVal: public std::integral_constant<T, v> {
public:
	using InnerType = T;

	template<class T2>
	requires(std::is_arithmetic_v<T2>)
	consteval operator T2() const {
		return T2(v);
	}

	consteval operator bool() const {
		return bool(v);
	}
};

template<class T, T v>
static constexpr const ConstVal<T, v> constval = {};

#define MAKE_BINARY_OPERATOR(op)                                     \
	template<class T1, class T2, T1 v1, T2 v2>                       \
	consteval auto operator op(ConstVal<T1, v1>, ConstVal<T2, v2>) { \
		return constval<decltype(v1 op v2), (v1 op v2)>;             \
	}

#define MAKE_UNARY_OPERATOR(op)                   \
	template<class T, T v1>                       \
	consteval auto operator op(ConstVal<T, v1>) { \
		return constval<T, op v1>;                \
	}

MAKE_BINARY_OPERATOR(+)
MAKE_BINARY_OPERATOR(-)
MAKE_BINARY_OPERATOR(*)
MAKE_BINARY_OPERATOR(/)
MAKE_BINARY_OPERATOR(%)
MAKE_BINARY_OPERATOR(^)
MAKE_BINARY_OPERATOR(&)
MAKE_BINARY_OPERATOR(|)
MAKE_BINARY_OPERATOR(==)
MAKE_BINARY_OPERATOR(!=)
MAKE_BINARY_OPERATOR(<)
MAKE_BINARY_OPERATOR(>)
MAKE_BINARY_OPERATOR(<=>)
MAKE_BINARY_OPERATOR(<=)
MAKE_BINARY_OPERATOR(>=)
MAKE_BINARY_OPERATOR(<<)
MAKE_BINARY_OPERATOR(>>)
MAKE_BINARY_OPERATOR(&&)
MAKE_BINARY_OPERATOR(||)
MAKE_UNARY_OPERATOR(~)
MAKE_UNARY_OPERATOR(!)
MAKE_UNARY_OPERATOR(+)
MAKE_UNARY_OPERATOR(-)
MAKE_UNARY_OPERATOR(++)
MAKE_UNARY_OPERATOR(--)

#undef MAKE_UNARY_OPERATOR
#undef MAKE_BINARY_OPERATOR

template<size_t v>
using ConstSize = ConstVal<size_t, v>;

template<size_t v>
static constexpr const ConstSize<v> constsize = {};

} // namespace arch::utils
