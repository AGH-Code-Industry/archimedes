#pragma once

#include "Append.hpp"
#include "AppendOther.hpp"
#include "Back.hpp"
#include "Cat.hpp"
#include "Contains.hpp"
#include "Distinct.hpp"
#include "DoN.hpp"
#include "EndsWith.hpp"
#include "Erase.hpp"
#include "EraseIf.hpp"
#include "Find.hpp"
#include "Front.hpp"
#include "Get.hpp"
#include "GetI.hpp"
#include "Insert.hpp"
#include "Iterator.hpp"
#include "Length.hpp"
#include "Placeholder.hpp"
#include "Pop.hpp"
#include "PopN.hpp"
#include "Prepend.hpp"
#include "PrependOther.hpp"
#include "RFind.hpp"
#include "Replace.hpp"
#include "Reverse.hpp"
#include "StartsWith.hpp"
#include "Substr.hpp"
#include <tuple>

namespace arch {

namespace tUtils {
namespace typeString {
template<class... Ts>
struct TypeString;

template<class Ph>
struct AsTypeString {
	using type = Ph;
};

template<class... Ts>
struct AsTypeString<_Ph<Ts...>> {
	using type = TypeString<Ts...>;
};

template<class TS>
struct AsPh {
	using type = TS;
};

template<class... Ts>
struct AsPh<TypeString<Ts...>> {
	using type = _Ph<Ts...>;
};

template<class... Ts>
struct TypeString {
private:
	template<class... Ts2>
	friend struct TypeString;

	using _PhT = _Ph<Ts...>;

public:
	using toTuple = std::tuple<Ts...>;

	template<class... Ts2>
	using append = typename AsTypeString<typename PhAppend<true, _PhT, Ts2...>::type>::type;
	template<class... TS>
	using appendOther = typename AsTypeString<typename PhAppendOther<_PhT, typename TS::_PhT...>::type>::type;

	template<class... Ts2>
	using prepend = typename AsTypeString<typename PhPrepend<true, _PhT, Ts2...>::type>::type;
	template<class... TS>
	using prependOther = typename AsTypeString<typename PhPrependOther<_PhT, typename TS::_PhT...>::type>::type;

	template<class... TS>
	using cat = typename AsTypeString<typename PhCat<true, _PhT, typename TS::_PhT...>::type>::type;

	template<class... Ts2>
	static inline constexpr bool contains = PhContains<_PhT, Ts2...>::value;
	template<class TS>
	static inline constexpr bool containsOther = PhContainsOther<_PhT, typename TS::_PhT>::value;
	template<class... Ts2>
	static inline constexpr bool containsAny = PhContainsAny<_PhT, Ts2...>::value;
	template<class... Ts2>
	static inline constexpr bool containsAll = PhContainsAllFrom<_PhT, _Ph<Ts2...>>::value;
	template<class TS>
	static inline constexpr bool containsAnyFrom = PhContainsAnyFrom<_PhT, typename TS::_PhT>::value;
	template<class TS>
	static inline constexpr bool containsAllFrom = PhContainsAllFrom<_PhT, typename TS::_PhT>::value;

	using distinct = typename AsTypeString<typename PhDistinct<_PhT>::type>::type;

	template<size_t Pos = 0, size_t Count = 1>
	using erase = typename AsTypeString<typename PhErase<_PhT, Pos, Count>::type>::type;
	template<class T, size_t Pos = 0>
	using eraseT = typename AsTypeString<typename PhEraseT<_PhT, T, Pos>::type>::type;
	template<template<class T> class Pred, size_t Pos = 0>
	using eraseIf = typename AsTypeString<typename PhEraseIf<_PhT, Pred, Pos>::type>::type;

	template<size_t Pos, class... Ts2>
	static inline constexpr size_t findAt = PhFindAt<true, _PhT, Pos, Ts2...>::value;
	template<class... Ts2>
	static inline constexpr size_t find = PhFind<true, _PhT, Ts2...>::value;
	template<class TS, size_t Pos = 0>
	static inline constexpr size_t findOther = PhFindOtherAt<true, _PhT, Pos, typename TS::_PhT>::value;

	template<size_t Pos, class... Ts2>
	static inline constexpr size_t findAnyAt = PhFindAnyAt<true, _PhT, Pos, Ts2...>::value;
	template<class... Ts2>
	static inline constexpr size_t findAny = PhFindAny<true, _PhT, Ts2...>::value;
	template<class TS, size_t Pos = 0>
	static inline constexpr size_t findAnyFrom = PhFindAnyFromAt<true, _PhT, Pos, typename TS::_PhT>::value;

	template<size_t Pos, class... Ts2>
	static inline constexpr size_t rFindAt = PhRFindAt<true, _PhT, Pos, Ts2...>::value;
	template<class... Ts2>
	static inline constexpr size_t rFind = PhRFind<true, _PhT, Ts2...>::value;
	template<class TS, size_t Pos = 0>
	static inline constexpr size_t rFindOther = PhRFindOtherAt<true, _PhT, Pos, typename TS::_PhT>::value;

	template<size_t Pos, class... Ts2>
	static inline constexpr size_t rFindAnyAt = PhRFindAnyAt<true, _PhT, Pos, Ts2...>::value;
	template<class... Ts2>
	static inline constexpr size_t rFindAny = PhRFindAny<true, _PhT, Ts2...>::value;
	template<class TS, size_t Pos = 0>
	static inline constexpr size_t rFindAnyFrom = PhRFindAnyFromAt<true, _PhT, Pos, typename TS::_PhT>::value;

	using front = typename PhFront<_PhT>::type;
	using back = typename PhBack<_PhT>::type;

	template<size_t Pos>
	using get = typename PhGet<_PhT, Pos>::type;

	template<size_t Pos, class... Ts2>
	using insert = typename AsTypeString<typename PhInsert<_PhT, Pos, Ts2...>::type>::type;
	template<size_t Pos, class... TS>
	using insertFrom = typename AsTypeString<typename PhInsertFrom<_PhT, Pos, typename TS::_PhT...>::type>::type;

	static inline constexpr size_t length = sizeof...(Ts);

	using popFront = typename AsTypeString<typename PhPopFront<true, _PhT>::type>::type;
	using popBack = typename AsTypeString<typename PhPopBack<true, _PhT>::type>::type;
	template<size_t N>
	using popNFront = typename AsTypeString<typename PhPopNFront<true, _PhT, N>::type>::type;
	template<size_t N>
	using popNBack = typename AsTypeString<typename PhPopNBack<true, _PhT, N>::type>::type;

	using reverse = typename AsTypeString<typename PhReverse<_PhT>::type>::type;

	template<class... Ts2>
	static inline constexpr bool startsWith = PhStartsWith<_PhT, Ts2...>::value;
	template<class TS>
	static inline constexpr bool startsWithOther = PhStartsWithOther<_PhT, typename TS::_PhT>::value;
	template<class... Ts2>
	static inline constexpr bool endsWith = PhEndsWith<_PhT, Ts2...>::value;
	template<class TS>
	static inline constexpr bool endsWithOther = PhEndsWithOther<_PhT, typename TS::_PhT>::value;

	template<size_t Pos = 0, size_t Count = -1>
	using substr = typename AsTypeString<typename PhSubstr<true, _PhT, Pos, Count>::type>::type;

	using id = TypeString<Ts...>;
};
} // namespace typeString

using typeString::TypeString;
} // namespace tUtils

using tUtils::TypeString;
} // namespace arch
