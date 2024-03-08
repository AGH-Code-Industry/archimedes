#pragma once

#include "placeholder.hpp"
#include "find.hpp"
#include "reverse.hpp"

namespace arch::ts {
template<bool R, class Ph, size_t Pos, class... Ts>
struct ph_rfind_at {
	static inline constexpr size_t value = -1;
};

template<size_t Pos, class... Ts, class... Ts2>
struct ph_rfind_at<true, ph<Ts...>, Pos, Ts2...> {
	using reversed = ph_reverse<ph<Ts...>>::type;
	using second_reversed = ph_reverse<ph<Ts2...>>::type;
	using substr = ph_substr<true, reversed, 0, ph_length<reversed>::value - ph_length<second_reversed>::value - 1>::type;
	static inline constexpr size_t found = ph_find_other<true, substr, second_reversed>::value;

	static inline constexpr size_t value = (found == (size_t)-1 ? -1 : ph_length<reversed>::value - found - ph_length<second_reversed>::value;
};

template<bool R, class Ph, size_t Pos, class Ph2>
struct ph_rfind_other_at;

template<class Ph, size_t Pos, class Ph2>
struct ph_rfind_other_at<false, Ph, Pos, Ph2> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class Ph2>
struct ph_rfind_other_at<true, Ph, Pos, Ph2> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class... Ts>
struct ph_rfind_other_at<true, Ph, Pos, ph<Ts...>> : ph_rfind_at<true, Ph, Pos, Ts...> {};

template<bool R, class Ph, class Ph2>
using ph_rfind_other = ph_rfind_other_at<R, Ph, 0, Ph2>;



template<bool R, class Ph, size_t Pos, class... Ts>
struct ph_rfind_any_at;

template<class Ph, size_t Pos, class... Ts>
struct ph_rfind_any_at<false, Ph, Pos, Ts...> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class... Ts>
struct ph_rfind_any_at<true, Ph, Pos, Ts...> {
	static inline constexpr size_t value = -1;
};

template<class... Ts, size_t Pos>
struct ph_rfind_any_at<true, ph<Ts...>, Pos> {
	static inline constexpr size_t value = -1;
};

template<class... Ts, size_t Pos, class T, class... Ts2>
struct ph_rfind_any_at<true, ph<Ts...>, Pos, T, Ts2...> {
	static inline constexpr bool found_first = ph_rfind_at<true, ph<Ts...>, Pos, T>::value != -1;
	static inline constexpr size_t value = (found_first ? ph_rfind_at<true, ph<Ts...>, Pos, T>::value : ph_rfind_any_at<not found_first, ph<Ts...>, Pos, Ts2...>::value);
};

template<bool R, class Ph, class... Ts>
using ph_rfind_any = ph_rfind_any_at<R, Ph, 0, Ts...>;

template<bool R, class Ph, size_t Pos, class Ph2>
struct ph_rfind_any_from_at;

template<class Ph, size_t Pos, class Ph2>
struct ph_rfind_any_from_at<false, Ph, Pos, Ph2> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class... Ts>
struct ph_rfind_any_from_at<true, Ph, Pos, ph<Ts...>> : ph_rfind_any_at<true, Ph, Pos, Ts...> {};

template<bool R, class Ph, class Ph2>
using ph_rfind_any_from = ph_rfind_any_from_at<R, Ph, 0, Ph2>;
}