#pragma once

#include "Find.hpp"
#include "Placeholder.hpp"
#include "Reverse.hpp"

namespace arch::tUtils::typeString {
template<bool R, class Ph, size_t Pos, class... Ts>
struct PhRFindAt {
	static inline constexpr size_t value = (size_t)-1;
};

template<size_t Pos, class... Ts, class... Ts2>
struct PhRFindAt<true, _Ph<Ts...>, Pos, Ts2...> {
	using reversed = PhReverse<_Ph<Ts...>>::type;
	using secondReversed = PhReverse<_Ph<Ts2...>>::type;
	using substr = PhSubstr<true, reversed, 0, PhLength<reversed>::value - PhLength<secondReversed>::value - 1>::type;
	static inline constexpr size_t found = PhFindOther<true, substr, secondReversed>::value;

	static inline constexpr size_t value =
		(found == (size_t)-1 ? -1 : PhLength<reversed>::value - found - PhLength<secondReversed>::value);
};

template<bool R, class Ph, class... Ts>
using PhRFind = PhRFindAt<R, Ph, 0, Ts...>;

template<bool R, class Ph, size_t Pos, class Ph2>
struct PhRFindOtherAt;

template<class Ph, size_t Pos, class Ph2>
struct PhRFindOtherAt<false, Ph, Pos, Ph2> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class Ph2>
struct PhRFindOtherAt<true, Ph, Pos, Ph2> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class... Ts>
struct PhRFindOtherAt<true, Ph, Pos, _Ph<Ts...>>: PhRFindAt<true, Ph, Pos, Ts...> {};

template<bool R, class Ph, class Ph2>
using PhRFindOther = PhRFindOtherAt<R, Ph, 0, Ph2>;

template<bool R, class Ph, size_t Pos, class... Ts>
struct PhRFindAnyAt;

template<class Ph, size_t Pos, class... Ts>
struct PhRFindAnyAt<false, Ph, Pos, Ts...> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class... Ts>
struct PhRFindAnyAt<true, Ph, Pos, Ts...> {
	static inline constexpr size_t value = -1;
};

template<class... Ts, size_t Pos>
struct PhRFindAnyAt<true, _Ph<Ts...>, Pos> {
	static inline constexpr size_t value = -1;
};

template<class... Ts, size_t Pos, class T, class... Ts2>
struct PhRFindAnyAt<true, _Ph<Ts...>, Pos, T, Ts2...> {
	static inline constexpr bool foundFirst = PhRFindAt<true, _Ph<Ts...>, Pos, T>::value != -1;
	static inline constexpr size_t value =
		(foundFirst ? PhRFindAt<true, _Ph<Ts...>, Pos, T>::value :
					  PhRFindAnyAt<not foundFirst, _Ph<Ts...>, Pos, Ts2...>::value);
};

template<bool R, class Ph, class... Ts>
using PhRFindAny = PhRFindAnyAt<R, Ph, 0, Ts...>;

template<bool R, class Ph, size_t Pos, class Ph2>
struct PhRFindAnyFromAt;

template<class Ph, size_t Pos, class Ph2>
struct PhRFindAnyFromAt<false, Ph, Pos, Ph2> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class... Ts>
struct PhRFindAnyFromAt<true, Ph, Pos, _Ph<Ts...>>: PhRFindAnyAt<true, Ph, Pos, Ts...> {};

template<bool R, class Ph, class Ph2>
using PhRFindAnyFrom = PhRFindAnyFromAt<R, Ph, 0, Ph2>;
} // namespace arch::tUtils::typeString
