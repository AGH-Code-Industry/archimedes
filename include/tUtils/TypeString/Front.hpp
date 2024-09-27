#pragma once

#include "Get.hpp"
#include "Placeholder.hpp"

namespace arch::tUtils::typeString {
template<class Ph>
using PhFront = PhGet<Ph, 0>;
} // namespace arch::tUtils::typeString
