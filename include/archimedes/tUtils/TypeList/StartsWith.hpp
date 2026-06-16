#pragma once

#include "Get.hpp"
#include "Placeholder.hpp"
#include "Pop.hpp"
#include "SubList.hpp"

namespace arch::tUtils::typeList {

/// @brief Checks if starts with type sequence
/// @tparam Ph - placeholder type
/// @tparam Ts - types to find
template<class Ph, class... Ts>
struct PhStartsWith {
private:

	using head = PhSubList<true, Ph, 0, sizeof...(Ts)>::type;

public:

	static inline constexpr bool value = std::same_as<head, _Ph<Ts...>>;
};

/// @brief Checks if starts with placeholder
/// @tparam Ph - placeholder type
/// @tparam Ph2 - placeholder to find
template<class Ph, class Ph2>
struct PhStartsWithOther {
	static inline constexpr bool value = false;
};

template<class Ph, class... Ts>
struct PhStartsWithOther<Ph, _Ph<Ts...>>: PhStartsWith<Ph, Ts...> {};

} // namespace arch::tUtils::typeList
