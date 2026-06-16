#pragma once

#include "ThisT.hpp"
#include <concepts>

namespace arch::meta::thisT {

/// @brief Helper concept, which checks if T had THIS_T specified inside
template<class T>
concept HasThisT = requires { typename T::ThisT; };

} // namespace arch::meta::thisT
