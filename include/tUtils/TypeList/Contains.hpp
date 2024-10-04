#pragma once

#include "Find.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeList {

/// @brief Checks if placeholder contains sublist of given types
/// @tparam Ph - placeholder type
/// @tparam Ts - types to check
template<class Ph, class... Ts>
struct PhContains {
	static inline constexpr bool value = PhFind<true, Ph, Ts...>::value != -1;
};

/// @brief Checks if placeholder contains given sublist
/// @tparam Ph - placeholder type
/// @tparam Ph2 - sublist to check
template<class Ph, class Ph2>
struct PhContainsOther {
	static inline constexpr bool value = false;
};

template<class Ph, class... Ts>
struct PhContainsOther<Ph, _Ph<Ts...>>: PhContains<Ph, Ts...> {};

/// @brief Checks if placeholder contains any of specified types
/// @tparam Ph - placeholder
/// @tparam Ts - types to check
template<class Ph, class... Ts>
struct PhContainsAny {
	static inline constexpr bool value = false;
};

template<class Ph>
struct PhContainsAny<Ph> {
	static inline constexpr bool value = false;
};

template<class Ph, class T, class... Ts>
struct PhContainsAny<Ph, T, Ts...> {
	static inline constexpr bool value = PhContains<Ph, T>::value or PhContainsAny<Ph, Ts...>::value;
};

/// @brief Checks if placeholder contains any of types from sublist
/// @tparam Ph - placeholder
/// @tparam Ts - types to check
template<class Ph, class Ph2>
struct PhContainsAnyFrom {
	static inline constexpr bool value = false;
};

template<class Ph, class T, class... Ts>
struct PhContainsAnyFrom<Ph, _Ph<T, Ts...>>: PhContainsAny<Ph, T, Ts...> {};

/// @brief Checks if placeholder contains all of types from sublist
/// @tparam Ph - placeholder
/// @tparam Ts - types to check
template<class Ph, class Ph2>
struct PhContainsAllFrom {
	static inline constexpr bool value = false;
};

template<class Ph, class... Ts>
struct PhContainsAllFrom<Ph, _Ph<Ts...>> {
	static inline constexpr bool value = (PhContains<Ph, Ts>::value && ...);
};

} // namespace arch::tUtils::typeList
