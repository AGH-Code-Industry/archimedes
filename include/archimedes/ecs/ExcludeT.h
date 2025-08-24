#pragma once

#include <archimedes/tUtils/TypeList.h>

namespace arch::ecs {

/// @brief Type indicating exclusion of components from views
/// @tparam Ts - components to exclude
template<class... Ts>
struct ExcludeT: public TypeList<Ts...> {};

/// @brief Instance of ExcludeT
/// @tparam Ts - components to exclude
template<class... Ts>
static inline constexpr ExcludeT<Ts...> exclude{};

} // namespace arch::ecs

namespace arch {

// expose exclude as arch::exclude
using ecs::exclude;
using ecs::ExcludeT;

} // namespace arch
