#pragma once

#include "Append.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeList {

/// @brief Concatenates placeholders
/// @tparam R - Whether to perform recursion
/// @tparam Ph - first placeholder
/// @tparam Phs - other placeholders
template<bool R, class Ph, class... Phs>
struct PhCat {
	using type = Ph;
};

template<class Ph, class... Phs>
struct PhCat<false, Ph, Phs...> {
	using type = RecursionEndT;
};

template<class Ph>
struct PhCat<true, Ph> {
	using type = Ph;
};

template<class Ph, class... Phs>
struct PhCat<true, Ph, Phs...>: PhCat<true, Phs...> {};

template<class... Ts, class Ph2, class... Phs>
struct PhCat<true, _Ph<Ts...>, Ph2, Phs...>: PhCat<true, _Ph<Ts...>, Phs...> {};

template<class... Ts, class... Ts2, class... Phs>
struct PhCat<true, _Ph<Ts...>, _Ph<Ts2...>, Phs...>: PhCat<true, _Ph<Ts..., Ts2...>, Phs...> {};

} // namespace arch::tUtils::typeList
