#pragma once

#include "placeholder.hpp"
#include "get.hpp"

namespace arch::ts {
template<class Ph>
using ph_front = ph_get<Ph, 0>;

template<class Ph>
using ph_back = ph_get<Ph, ph_length<Ph>::value - 1>;
}