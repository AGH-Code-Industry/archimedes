#pragma once

namespace arch::utils {

template<class... Types>
struct TypeList;

template<class... Types>
static inline constexpr const TypeList<Types...> typelist = {};

} // namespace arch::utils
