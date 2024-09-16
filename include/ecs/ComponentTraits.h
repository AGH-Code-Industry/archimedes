#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include "ComponentSpecs.h"
#include "EntityTraits.h"
#include "Flag.h"

namespace arch::ecs::_details { // NOLINT

/// @brief Contains info, constants and operations on components and their pages
/// @details All data extracted from ComponentSpecs
/// @tparam C - component type
/// @tparam E - entity type
template<class C, class E>
struct ComponentTraits {
	/// @brief Component Specifications
	using Specs = ComponentSpecs<C>;

	/// @brief Type of component
	using ComponentT = C;

	/// @brief Whether component is marked as in-place
	static inline constexpr bool inPlace = Specs::inPlace;
	/// @brief Whether component is movable
	static inline constexpr bool movable = std::movable<ComponentT>;
	static_assert(not(not movable and not inPlace), "Non-movable components cannot be marked as not-in-place");
	/// @brief Component page size
	static inline constexpr size_t pageSize = Specs::pageSize;
	static_assert(std::popcount(pageSize) == 1, "pageSize must be a power of 2");

	/// @brief Size of component in bytes
	static inline constexpr size_t size = sizeof(ComponentT);
	/// @brief Alignment of component
	static inline constexpr size_t alignment = alignof(ComponentT);

	/// @brief Whether component is a flag component,
	static inline constexpr bool flag = FlagComponent<ComponentT>;
	static_assert(not(flag and not std::is_empty_v<ComponentT>), "Non-empty type cannot be marked as flag-component");

	/// @brief Creates new page of size pageSize
	static inline ComponentT* newPage() noexcept;
	/// @brief Destroys all components and deallocates memory of given page
	/// @param pages - pointer to all pages
	/// @param pageNum - index of page in pages
	/// @param dense - dense vector of entities
	static inline void deletePage(ComponentT** pages, size_t pageNum, const std::vector<E>& dense) noexcept;
	/// @brief Destroys component at given location
	/// @param component - component to destroy
	static inline void destroyAt(ComponentT* component) noexcept;
	/// @brief Constructs component on given location with given arguments
	/// @param component - pointer to location where to construct component
	/// @param ...args - arguments for constructor
	template<class... Args>
	static inline ComponentT& constructAt(ComponentT* component, Args&&... args) noexcept;
};

} // namespace arch::ecs::_details

#include "ComponentTraits.hpp"
