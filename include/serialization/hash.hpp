#pragma once

#include <string>

namespace arch::ser {
/// @brief Simple string hashing function.
/// @param name - string to hash.
size_t hash(const std::string& name) noexcept;
}