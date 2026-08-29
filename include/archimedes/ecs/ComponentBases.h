#pragma once

namespace arch::ecs {

/// @brief Helper class for marking in-place components
struct InPlaceComponent {};

/// @brief Helper class for marking flag-components
struct FlagComponent {};

namespace _details {

/// @brief Base of ComponentPageSize
struct ComponentPageSizeBase {};

} // namespace _details

/// @brief Helper class for setting non-default component page sizes
/// @tparam PageSize - component page size, must be a power of 2
template<size_t PageSize>
struct ComponentPageSize: _details::ComponentPageSizeBase {
	static_assert(std::popcount(PageSize) == 1, "Page size must be a power of 2");

	static constexpr size_t pageSize = PageSize;
};

} // namespace arch::ecs
