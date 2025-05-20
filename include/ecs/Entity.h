#pragma once

#include <cstddef>
#include <cstdint>

namespace arch::ecs {

/// @brief Entity type
/// @details 48-bit id (281'474'976'710'655 ids)
/// @details 16-bit version (65'535)
/// @details page size: 1024
enum class Entity : uint64_t; /* NOLINT */

} // namespace arch::ecs
