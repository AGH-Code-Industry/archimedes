#pragma once

#include <string>

namespace arch {
namespace meta {
namespace _nameOfType {

/// @brief Standardises names returned by nameOf<T>()
/// @param name - name to standardize
std::string nameOfTypeFmt(std::string_view name) noexcept;

} // namespace _nameOfType

using _nameOfType::nameOfTypeFmt;
} // namespace meta

using meta::nameOfTypeFmt;
} // namespace arch
