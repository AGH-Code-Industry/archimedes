#pragma once

#include "Append.hpp"
#include "AppendOther.hpp"
#include "Back.hpp"
#include "Cat.hpp"
#include "Contains.hpp"
#include "Distinct.hpp"
#include "EndsWith.hpp"
#include "Erase.hpp"
#include "EraseIf.hpp"
#include "Filter.hpp"
#include "Find.hpp"
#include "Front.hpp"
#include "Get.hpp"
#include "Insert.hpp"
#include "Length.hpp"
#include "Placeholder.hpp"
#include "Pop.hpp"
#include "PopN.hpp"
#include "Prepend.hpp"
#include "PrependOther.hpp"
#include "Reverse.hpp"
#include "StartsWith.hpp"
#include "SubList.hpp"
#include "Transform.hpp"
#include <tuple>

namespace arch {

namespace tUtils {
namespace typeList {
template<class... Ts>
struct TypeList;

/// @brief Converts placeholder to TypeList
/// @tparam Ph
template<class Ph>
struct AsTypeList {
	using type = Ph;
};

template<class... Ts>
struct AsTypeList<_Ph<Ts...>> {
	using type = TypeList<Ts...>;
};

/// @brief Converts TypeList to placeholder
/// @tparam TL
template<class TL>
struct AsPh {
	using type = TL;
};

template<class... Ts>
struct AsPh<TypeList<Ts...>> {
	using type = _Ph<Ts...>;
};

/// @brief Struct resembling tuple, allowing various operations on contained types
/// @tparam Ts - contained types
template<class... Ts>
struct TypeList {
private:
	template<class... Ts2>
	friend struct TypeList;

	using _PhT = _Ph<Ts...>;

public:
	/// @brief Converts TypeList to std::tuple
	using toTuple = std::tuple<Ts...>;

	/// @brief Appends types
	/// @tparam Ts2 - types to append
	template<class... Ts2>
	using append = typename AsTypeList<typename PhAppend<true, _PhT, Ts2...>::type>::type;
	/// @brief Appends types from other TypeLists
	/// @tparam TLs - TypeLists to append
	template<class... TLs>
	using appendOther = typename AsTypeList<typename PhAppendOther<_PhT, typename TLs::_PhT...>::type>::type;

	template<class... Ts2>
	using prepend = typename AsTypeList<typename PhPrepend<true, _PhT, Ts2...>::type>::type;
	template<class... TS>
	using prependOther = typename AsTypeList<typename PhPrependOther<_PhT, typename TS::_PhT...>::type>::type;

	template<class... TS>
	using cat = typename AsTypeList<typename PhCat<true, _PhT, typename TS::_PhT...>::type>::type;

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

	using distinct = typename AsTypeList<typename PhDistinct<_PhT>::type>::type;

	template<size_t Pos = 0, size_t Count = 1>
	using erase = typename AsTypeList<typename PhErase<_PhT, Pos, Count>::type>::type;
	template<class T, size_t Pos = 0>
	using eraseT = typename AsTypeList<typename PhEraseT<_PhT, T, Pos>::type>::type;
	template<template<class T> class Pred, size_t Pos = 0>
	using eraseIf = typename AsTypeList<typename PhEraseIf<_PhT, Pred, Pos>::type>::type;

	template<template<class> class Fn, size_t Pos = 0>
	using transform = typename AsTypeList<typename PhTransform<_PhT, Fn, Pos>::type>::type;
	template<template<class> class Pred, size_t Pos = 0>
	using filter = typename AsTypeList<typename PhFilter<_PhT, Pred, Pos>::type>::type;

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

	using front = typename PhFront<_PhT>::type;
	using back = typename PhBack<_PhT>::type;

	template<size_t Pos>
	using get = typename PhGet<_PhT, Pos>::type;

	template<size_t Pos, class... Ts2>
	using insert = typename AsTypeList<typename PhInsert<_PhT, Pos, Ts2...>::type>::type;
	template<size_t Pos, class... TS>
	using insertFrom = typename AsTypeList<typename PhInsertFrom<_PhT, Pos, typename TS::_PhT...>::type>::type;

	static inline constexpr size_t length = sizeof...(Ts);

	using popFront = typename AsTypeList<typename PhPopFront<true, _PhT>::type>::type;
	using popBack = typename AsTypeList<typename PhPopBack<true, _PhT>::type>::type;
	template<size_t N>
	using popNFront = typename AsTypeList<typename PhPopNFront<true, _PhT, N>::type>::type;
	template<size_t N>
	using popNBack = typename AsTypeList<typename PhPopNBack<true, _PhT, N>::type>::type;

	using reverse = typename AsTypeList<typename PhReverse<_PhT>::type>::type;

	template<class... Ts2>
	static inline constexpr bool startsWith = PhStartsWith<_PhT, Ts2...>::value;
	template<class TS>
	static inline constexpr bool startsWithOther = PhStartsWithOther<_PhT, typename TS::_PhT>::value;
	template<class... Ts2>
	static inline constexpr bool endsWith = PhEndsWith<_PhT, Ts2...>::value;
	template<class TS>
	static inline constexpr bool endsWithOther = PhEndsWithOther<_PhT, typename TS::_PhT>::value;

	template<size_t Pos = 0, size_t Count = -1>
	using substr = typename AsTypeList<typename PhSubList<true, _PhT, Pos, Count>::type>::type;

	using id = TypeList<Ts...>;
};
} // namespace typeList

using typeList::TypeList;
} // namespace tUtils

namespace typeList {
using NoneT = tUtils::typeList::NoneT;
}

using tUtils::TypeList;
} // namespace arch
