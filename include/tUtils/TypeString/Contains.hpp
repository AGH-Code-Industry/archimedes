#pragma once

#include "Find.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeString {
template<class Ph, class... Ts>
struct PhContains {
	static inline constexpr bool value = PhFind<true, Ph, Ts...>::value != -1;
};

template<class Ph, class Ph2>
struct PhContainsOther {
	static inline constexpr bool value = false;
};

template<class Ph, class... Ts>
struct PhContainsOther<Ph, _Ph<Ts...>>: PhContains<Ph, Ts...> {};

template<class Ph, class... Ts>
struct PhContainsAny;

template<class Ph>
struct PhContainsAny<Ph> {
	static inline constexpr bool value = false;
};

template<class Ph, class T, class... Ts>
struct PhContainsAny<Ph, T, Ts...> {
	static inline constexpr bool value = PhContains<Ph, T>::value or PhContainsAny<Ph, Ts...>::value;
};

template<class Ph, class Ph2>
struct PhContainsAnyFrom {
	static inline constexpr bool value = false;
};

template<class Ph, class T, class... Ts>
struct PhContainsAnyFrom<Ph, _Ph<T, Ts...>>: PhContainsAny<Ph, T, Ts...> {};

template<class Ph, class Ph2>
struct PhContainsAllFrom {
	static inline constexpr bool value = false;
};

template<class Ph, class... Ts>
struct PhContainsAllFrom<Ph, _Ph<Ts...>> {
	static inline constexpr bool value = (PhContains<Ph, Ts>::value && ...);
};
} // namespace arch::tUtils::typeString
