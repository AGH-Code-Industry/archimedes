#pragma once

#include "DoN.hpp"
#include "Iterator.hpp"
#include "Placeholder.hpp"
#include "PopN.hpp"

namespace arch::tUtils::typeString {
template<bool R, class Ph, size_t Pos, size_t Count>
struct PhSubstr {
	using type = Ph;
};

template<class Ph, size_t Pos, size_t Count>
struct PhSubstr<false, Ph, Pos, Count> {
	using type = typeStringRecursionEnd;
};

template<class... Ts, size_t Pos, size_t Count>
struct PhSubstr<true, _Ph<Ts...>, Pos, Count> {
	using headless = PhPopNFront<true, _Ph<Ts...>, Pos>::type;
	static inline constexpr size_t tailLength =
		(PhLength<headless>::value > Count ? PhLength<headless>::value - Count : 0);

	using type = PhPopNBack<true, headless, tailLength>::type;
};
} // namespace arch::tUtils::typeString
