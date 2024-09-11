#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include "ComponentSpecs.h"
#include "EntityTraits.h"

namespace arch::ecs::_details { // NOLINT

template<class C, class E>
struct ComponentTraits {
	using Specs = ComponentSpecs<C>;

	using Type = C;

	static inline constexpr bool inPlace = Specs::inPlace;
	static inline constexpr bool movable = std::movable<C>;
	static_assert(movable or inPlace, "Non-movable components cannot be marked as not-in-place");
	static inline constexpr size_t pageSize = Specs::pageSize;
	static_assert(std::popcount(pageSize) == 1, "pageSize must be a power of 2");

	static inline constexpr size_t size = sizeof(C);
	static inline constexpr size_t alignment = alignof(C);

	static inline constexpr bool flag = (size == 0);

	static inline char* newPage() noexcept;
	static inline void deletePage(C** pages, size_t pageNum, const std::vector<E>& dense) noexcept;
	static inline void destroyAt(Type* component) noexcept;
	template<class... Args>
	static inline Type* createAt(Type* component, Args&&... args) noexcept;
};

} // namespace arch::ecs::_details

#include "ComponentTraits.hpp"
