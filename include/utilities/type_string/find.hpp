#pragma once

#include "placeholder.hpp"
#include "length.hpp"
#include "pop.hpp"
#include "starts_ends_with.hpp"
#include "reverse.hpp"

namespace arch::ts {
template<bool R, class Ph, size_t Pos, class... Ts>
struct ph_find_at;

template<class Ph, size_t Pos, class... Ts>
struct ph_find_at<false, Ph, Pos, Ts...> {
	static inline constexpr size_t value = -1;
};

template<class Ph, class... Ts>
struct ph_find_at<true, Ph, 0, Ts...> {
	static inline constexpr bool starts_with = ph_starts_with<Ph, Ts...>::value;
	static inline constexpr bool do_search_forward = not starts_with and ph_length<Ph>::value > sizeof...(Ts);
	using next = ph_pop_front<do_search_forward, Ph>::type;
	static inline constexpr size_t found_forward = ph_find_at<do_search_forward, next, 0, Ts...>::value;

	static inline constexpr size_t value = (do_search_forward ? (found_forward != (size_t)-1 ? found_forward + 1 : -1) : (starts_with ? 0 : -1));
};

template<class Ph, size_t Pos, class... Ts>
struct ph_find_at<true, Ph, Pos, Ts...> {
	using found = ph_find_at<true, typename ph_substr<true, Ph, Pos, -1>::type, 0, Ts...>;

	static inline constexpr size_t value = (found::value != (size_t)-1 ? found::value + Pos : (size_t)-1);
};

template<bool R, class Ph, class... Ts>
using ph_find = ph_find_at<R, Ph, 0, Ts...>;

template<bool R, class Ph, size_t Pos, class Ph2>
struct ph_find_other_at;

template<class Ph, size_t Pos, class Ph2>
struct ph_find_other_at<false, Ph, Pos, Ph2> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class Ph2>
struct ph_find_other_at<true, Ph, Pos, Ph2> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class... Ts>
struct ph_find_other_at<true, Ph, Pos, ph<Ts...>> : ph_find_at<true, Ph, Pos, Ts...> {};

template<bool R, class Ph, class Ph2>
using ph_find_other = ph_find_other_at<R, Ph, 0, Ph2>;



template<bool R, class Ph, size_t Pos, class... Ts>
struct ph_find_any_at;

template<class Ph, size_t Pos, class... Ts>
struct ph_find_any_at<false, Ph, Pos, Ts...> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class... Ts>
struct ph_find_any_at<true, Ph, Pos, Ts...> {
	static inline constexpr size_t value = -1;
};

template<class... Ts, size_t Pos>
struct ph_find_any_at<true, ph<Ts...>, Pos> {
	static inline constexpr size_t value = -1;
};

template<class... Ts, size_t Pos, class T, class... Ts2>
struct ph_find_any_at<true, ph<Ts...>, Pos, T, Ts2...> {
	static inline constexpr bool found_first = ph_find_at<true, ph<Ts...>, Pos, T>::value != -1;
	static inline constexpr size_t value = (found_first ? ph_find_at<true, ph<Ts...>, Pos, T>::value : ph_find_any_at<not found_first, ph<Ts...>, Pos, Ts2...>::value);
};

template<bool R, class Ph, class... Ts>
using ph_find_any = ph_find_any_at<R, Ph, 0, Ts...>;

template<bool R, class Ph, size_t Pos, class Ph2>
struct ph_find_any_from_at;

template<class Ph, size_t Pos, class Ph2>
struct ph_find_any_from_at<false, Ph, Pos, Ph2> {
	static inline constexpr size_t value = -1;
};

template<class Ph, size_t Pos, class... Ts>
struct ph_find_any_from_at<true, Ph, Pos, ph<Ts...>> : ph_find_any_at<true, Ph, Pos, Ts...> {};

template<bool R, class Ph, class Ph2>
using ph_find_any_from = ph_find_any_from_at<R, Ph, 0, Ph2>;
}