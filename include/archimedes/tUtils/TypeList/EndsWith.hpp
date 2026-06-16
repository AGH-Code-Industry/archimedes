#pragma once

#include "Get.hpp"
#include "Length.hpp"
#include "Placeholder.hpp"
#include "Pop.hpp"
#include "SubList.hpp"

namespace arch::tUtils::typeList {

/// @brief Checks if ends with type sequence
/// @tparam Ph - placeholder type
/// @tparam Ts - types to find
template<class Ph, class... Ts>
struct PhEndsWith {
private:

	using tail = PhSubList<true, Ph, PhLength<Ph>::value - sizeof...(Ts), PhLength<Ph>::value>::type;

public:

	static inline constexpr bool value = std::same_as<tail, _Ph<Ts...>>;
};

/// @brief Checks if ends with types from placeholder
/// @tparam Ph - placeholder type
/// @tparam Ph2 - placeholder to find
template<class Ph, class Ph2>
struct PhEndsWithOther {
	static inline constexpr bool value = false;
};

template<class Ph, class... Ts>
struct PhEndsWithOther<Ph, _Ph<Ts...>>: PhEndsWith<Ph, Ts...> {};

} // namespace arch::tUtils::typeList
