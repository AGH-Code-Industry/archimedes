#pragma once

#include "EndsWith.hpp"
#include "Length.hpp"
#include "Placeholder.hpp"
#include "Pop.hpp"
#include "Reverse.hpp"
#include "StartsWith.hpp"

namespace arch::tUtils::typeList {

/// @brief Finds type sequence
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam Pos - where to stant searching
/// @tparam Ts - types to find
template<bool R, class Ph, size_t Pos, class... Ts>
struct PhFindAt;

template<class Ph, size_t Pos, class... Ts>
struct PhFindAt<false, Ph, Pos, Ts...> {
	static inline constexpr size_t value = (size_t)-1;
};

template<class Ph, class... Ts>
struct PhFindAt<true, Ph, 0, Ts...> {
private:

	static inline constexpr bool startsWith = PhStartsWith<Ph, Ts...>::value;
	static inline constexpr bool doSearchForward = !startsWith && PhLength<Ph>::value > sizeof...(Ts);
	using next = PhPopFront<doSearchForward, Ph>::type;
	static inline constexpr size_t foundForward = PhFindAt<doSearchForward, next, 0, Ts...>::value;

public:

	static inline constexpr size_t value =
		(doSearchForward ? (foundForward != (size_t)-1 ? foundForward + 1 : (size_t)-1) : (startsWith ? 0 : -1));
};

template<class Ph, size_t Pos, class... Ts>
struct PhFindAt<true, Ph, Pos, Ts...> {
private:

	using found = PhFindAt<true, typename PhSubList<true, Ph, Pos, (size_t)-1>::type, 0, Ts...>;

public:

	static inline constexpr size_t value = (found::value != (size_t)-1 ? found::value + Pos : (size_t)-1);
};

/// @brief Finds type sequence from the beginning
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam Ts - types to find
template<bool R, class Ph, class... Ts>
using PhFind = PhFindAt<R, Ph, 0, Ts...>;

/// @brief Finds placeholder
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam Pos - where to stant searching
/// @tparam Ts - types to find
template<bool R, class Ph, size_t Pos, class Ph2>
struct PhFindOtherAt;

template<class Ph, size_t Pos, class Ph2>
struct PhFindOtherAt<false, Ph, Pos, Ph2> {
	static inline constexpr size_t value = (size_t)-1;
};

template<class Ph, size_t Pos, class Ph2>
struct PhFindOtherAt<true, Ph, Pos, Ph2> {
	static inline constexpr size_t value = (size_t)-1;
};

template<class Ph, size_t Pos, class... Ts>
struct PhFindOtherAt<true, Ph, Pos, _Ph<Ts...>>: PhFindAt<true, Ph, Pos, Ts...> {};

/// @brief Finds placeholder from the beginning
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam Ts - types to find
template<bool R, class Ph, class Ph2>
using PhFindOther = PhFindOtherAt<R, Ph, 0, Ph2>;

/// @brief Finds any type from sequence
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam Pos - where to stant searching
/// @tparam Ts - types to find
template<bool R, class Ph, size_t Pos, class... Ts>
struct PhFindAnyAt;

template<class Ph, size_t Pos, class... Ts>
struct PhFindAnyAt<false, Ph, Pos, Ts...> {
	static inline constexpr size_t value = (size_t)-1;
};

template<class Ph, size_t Pos, class... Ts>
struct PhFindAnyAt<true, Ph, Pos, Ts...> {
	static inline constexpr size_t value = (size_t)-1;
};

template<class... Ts, size_t Pos>
struct PhFindAnyAt<true, _Ph<Ts...>, Pos> {
	static inline constexpr size_t value = -1;
};

template<class... Ts, size_t Pos, class T, class... Ts2>
struct PhFindAnyAt<true, _Ph<Ts...>, Pos, T, Ts2...> {
private:

	static inline constexpr bool foundFirst = PhFindAt<true, _Ph<Ts...>, Pos, T>::value != -1;

public:

	static inline constexpr size_t value =
		(foundFirst ? PhFindAt<true, _Ph<Ts...>, Pos, T>::value :
					  PhFindAnyAt<!foundFirst, _Ph<Ts...>, Pos, Ts2...>::value);
};

/// @brief Finds any type from sequence from the beginning
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam Ts - types to find
template<bool R, class Ph, class... Ts>
using PhFindAny = PhFindAnyAt<R, Ph, 0, Ts...>;

/// @brief Finds any type from placeholder
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam Pos - where to stant searching
/// @tparam Ts - types to find
template<bool R, class Ph, size_t Pos, class Ph2>
struct PhFindAnyFromAt;

template<class Ph, size_t Pos, class Ph2>
struct PhFindAnyFromAt<false, Ph, Pos, Ph2> {
	static inline constexpr size_t value = (size_t)-1;
};

template<class Ph, size_t Pos, class... Ts>
struct PhFindAnyFromAt<true, Ph, Pos, _Ph<Ts...>>: PhFindAnyAt<true, Ph, Pos, Ts...> {};

/// @brief Finds any type from placeholder from the beginning
/// @tparam R - whether to continue recursion
/// @tparam Ph - placeholder type
/// @tparam Ts - types to find
template<bool R, class Ph, class Ph2>
using PhFindAnyFrom = PhFindAnyFromAt<R, Ph, 0, Ph2>;

} // namespace arch::tUtils::typeList
