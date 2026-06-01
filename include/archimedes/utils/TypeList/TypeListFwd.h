#pragma once

namespace arch::utils {

/// @brief Type collection, allowing for manipulation of type sequences
/// @tparam Types... - stored types
template<class... Types>
struct TypeList;

/// @brief Helper instance of TypeList
/// @tparam Types... - stored types
template<class... Types>
static inline constexpr const TypeList<Types...> typelist = {};

} // namespace arch::utils
