#pragma once

#include <cstddef>
#include <cstdint>

namespace arch::ecs {

/// @brief 32-bit entity type
/// @details 24-bit id (16'777'215 ids)
/// @details 8-bit version (255)
/// @details page size: 1024
enum class e32 : uint32_t; /* NOLINT */

/// @brief 64-bit entity type
/// @details 48-bit id (281'474'976'710'655 ids)
/// @details 16-bit version (65'535)
/// @details page size: 1024
enum class e64 : uint64_t; /* NOLINT */

} // namespace arch::ecs
