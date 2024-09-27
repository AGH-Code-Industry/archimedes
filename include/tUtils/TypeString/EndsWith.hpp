#pragma once

#include "Get.hpp"
#include "Placeholder.hpp"
#include "Pop.hpp"
#include "Substr.hpp"

namespace arch::tUtils::typeString {
template<class Ph, class... Ts>
struct PhEndsWith {
	using tail = PhSubstr<true, Ph, PhLength<Ph>::value - sizeof...(Ts), PhLength<Ph>::value>::type;

	static inline constexpr bool value = std::same_as<tail, _Ph<Ts...>>;
};

template<class Ph, class Ph2>
struct PhEndsWithOther {
	static inline constexpr bool value = false;
};

template<class Ph, class... Ts>
struct PhEndsWithOther<Ph, _Ph<Ts...>>: PhEndsWith<Ph, Ts...> {};
} // namespace arch::tUtils::typeString
