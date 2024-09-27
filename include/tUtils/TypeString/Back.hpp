#pragma once

#include "Get.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeString {
template<class Ph>
using PhBack = PhGet<Ph, PhLength<Ph>::value - 1>;
} // namespace arch::tUtils::typeString
