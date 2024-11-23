#pragma once

#include "ThisT.hpp"

namespace arch::meta::thisT {

/// @brief Helper class used in RTTIEnabled concept
template<class T>
struct UniqueGetter {
	using type = typename T::_ARCH_THIS_T_UNIQUE;
};

} // namespace arch::meta::thisT
