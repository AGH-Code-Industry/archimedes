#pragma once

#include "Length.hpp"
#include "Placeholder.hpp"
#include "PopN.hpp"

namespace arch::tUtils::typeList {

/// @brief Returns list with types at [Pos, Pos + Count)
/// @tparam R - whether to advance recursion
/// @tparam Ph - placeholder type
/// @tparam Pos - beginning of sublist
/// @tparam Count - ending of sublist
template<bool R, class Ph, size_t Pos, size_t Count>
struct PhSubList {
	using type = Ph;
};

template<class Ph, size_t Pos, size_t Count>
struct PhSubList<false, Ph, Pos, Count> {
	using type = RecursionEndT;
};

template<class... Ts, size_t Pos, size_t Count>
struct PhSubList<true, _Ph<Ts...>, Pos, Count> {
private:

	using headless = PhPopNFront<true, _Ph<Ts...>, Pos>::type;
	static inline constexpr size_t tailLength =
		(PhLength<headless>::value > Count ? PhLength<headless>::value - Count : 0);

public:

	using type = PhPopNBack<true, headless, tailLength>::type;
};

} // namespace arch::tUtils::typeList
