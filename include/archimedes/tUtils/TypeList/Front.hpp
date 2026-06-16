#pragma once

#include "Get.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeList {

/// @brief Obtains first type from placeholder
/// @tparam Ph - placeholder type
template<class Ph>
using PhFront = PhGet<Ph, 0>;

} // namespace arch::tUtils::typeList
