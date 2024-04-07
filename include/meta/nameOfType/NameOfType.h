#pragma once

#include <source_location>
#include <string_view>

namespace arch {
namespace meta {
namespace _nameOfType { // NOLINT(*)
/// @brief Helper function, which signature is used to determine name of type
template<class T>
constexpr std::string_view signature() noexcept = delete;

/// @brief Returns implementation-dependent name of type T
/// @brief To standardize output use nameOfFmt()
/// @tparam T - type to get type of
template<class T>
constexpr std::string_view nameOf() noexcept;

} // namespace _nameOfType
} // namespace meta
} // namespace arch

#include "NameOfType.hpp"
