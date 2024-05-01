#pragma once

#include <string>

namespace arch::meta::nameOfType {

/// @brief Standardises names returned by nameOf<T>()
/// @param name - name to standardize
std::string nameOfFmt(std::string_view name) noexcept;

} // namespace arch::meta::nameOfType
