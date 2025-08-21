#pragma once

#include <string_view>

namespace arch::meta::rtti {
/// @brief Type name hashing function, guarantees same output on different machines
/// @details Uses cyclic polynomial method
/// @param typeName - type name to hash
size_t hashTypeName(std::string_view typeName) noexcept;
} // namespace arch::meta::rtti
