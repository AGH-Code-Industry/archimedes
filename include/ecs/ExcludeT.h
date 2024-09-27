#pragma once

#include <tUtils/TypeString.h>

namespace arch::ecs {

/// @brief Type indicating exclusion of components from views
/// @tparam Ts - components to exclude
template<class... Ts>
struct ExcludeT: public TypeString<Ts...> {};

/// @brief Instance of ExcludeT
/// @tparam Ts - components to exclude
template<class... Ts>
static inline constexpr ExcludeT<Ts...> exclude{};

} // namespace arch::ecs

namespace arch {
using ecs::exclude;
}
