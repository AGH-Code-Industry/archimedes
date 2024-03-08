#pragma once

#include <type_traits>

namespace arch::ts {
template<class... Ts>
struct ph {};

template<bool B, class TT, class TF>
using cond_t = std::conditional_t<B, TT, TF>;

struct type_string_recursion_end {};
struct type_string_none_type {};
}