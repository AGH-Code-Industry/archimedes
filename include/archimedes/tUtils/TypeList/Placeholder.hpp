#pragma once

#include <type_traits>

namespace arch::tUtils::typeList {

/// @brief Placeholder type
/// @tparam Ts - types held
template<class... Ts>
struct _Ph {};

/// @brief Type indicating end of recursion
struct RecursionEndT {};

/// @brief Type indicating a null result of get operation
struct NoneT {};

} // namespace arch::tUtils::typeList
