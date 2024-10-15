#pragma once

#include "Get.hpp"
#include "Length.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeList {

/// @brief Obtains last type from placeholder
/// @tparam Ph - placeholder type
template<class Ph>
using PhBack = PhGet<Ph, PhLength<Ph>::value - 1>;

} // namespace arch::tUtils::typeList
