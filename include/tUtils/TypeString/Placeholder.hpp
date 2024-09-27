#pragma once

#include <type_traits>

namespace arch::tUtils::typeString {
template<class... Ts>
struct _Ph {};

template<bool B, class TT, class TF>
using CondT = std::conditional_t<B, TT, TF>;

struct typeStringRecursionEnd {};

struct typeStringNoneType {};
} // namespace arch::tUtils::typeString
