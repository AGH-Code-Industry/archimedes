#pragma once

#include "ThisTUnique.hpp"
#include <concepts>

namespace arch::meta::thisT {

/// @brief Helper concept, which checks if current class was specified in THIS_T(T)
/// @tparam T - T specified in THIS_T
/// @tparam Unique - struct _ThisTUnique from current class
template<class T, class Unique>
concept ThisTCheck = std::same_as<typename T::_ARCH_THIS_T_UNIQUE, Unique>;

} // namespace arch::meta::thisT
