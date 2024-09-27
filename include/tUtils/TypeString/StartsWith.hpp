#pragma once

#include "Get.hpp"
#include "Placeholder.hpp"
#include "Pop.hpp"
#include "Substr.hpp"

namespace arch::tUtils::typeString {
template<class Ph, class... Ts>
struct PhStartsWith {
	using head = PhSubstr<true, Ph, 0, sizeof...(Ts)>::type;

	static inline constexpr bool value = std::same_as<head, _Ph<Ts...>>;
};

template<class Ph, class Ph2>
struct PhStartsWithOther {
	static inline constexpr bool value = false;
};

template<class Ph, class... Ts>
struct PhStartsWithOther<Ph, _Ph<Ts...>>: PhStartsWith<Ph, Ts...> {};
} // namespace arch::tUtils::typeString
