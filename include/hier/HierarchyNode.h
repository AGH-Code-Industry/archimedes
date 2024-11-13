#pragma once

#include <ranges>

#include "ChildNode.h"
#include "ChildrenIterator.h"
#include "HierarchyIterator.h"
#include <ecs/ComponentTraits.h>

namespace arch::hier {

/// @brief Main node class, implementing hierarchies
/// @details An in-place component
struct HierarchyNode: public ChildNode {
	/// @brief Makes HierarchyNode immovable in ecs storage, to preserve internal pointers
	static constexpr bool inPlaceComponent = true;

	/// @brief Entity constructor
	HierarchyNode(const ecs::Entity entity) noexcept;

	/// @brief Sentinel node, before first and after the last
	ChildNode sentinel{ &sentinel, &sentinel, ecs::nullEntity };
	/// @brief First child node
	ChildNode* first = nullptr;
	/// @brief Last child node
	ChildNode* last = nullptr;
	/// @brief Children count
	u32 count = 0;
	/// @brief Hierarchy depth (distance to root)
	u32 depth = 0;
	/// @brief Parent node
	HierarchyNode* parent = nullptr;

	/// @brief Iterator type
	using Iterator = ChildrenIterator;
	/// @brief ReverseIterator type
	using ReverseIterator = std::reverse_iterator<ChildrenIterator>;

	/// @brief Returns iterator to first child
	Iterator begin() const noexcept;
	/// @brief Returns iterator to first child
	Iterator cbegin() const noexcept;
	/// @brief Returns iterator to sentinel
	Iterator end() const noexcept;
	/// @brief Returns iterator to sentinel
	Iterator cend() const noexcept;
	/// @brief Returns reverse iterator to last child
	ReverseIterator rbegin() const noexcept;
	/// @brief Returns reverse iterator to last child
	ReverseIterator crbegin() const noexcept;
	/// @brief Returns reverse iterator to sentinel
	ReverseIterator rend() const noexcept;
	/// @brief Returns reverse iterator to sentinel
	ReverseIterator crend() const noexcept;

	/// @brief Return view with recursive iterators
	auto recursiveIterable() noexcept
		-> decltype(std::ranges::subrange(_details::HierarchyIterator(first), _details::HierarchyIterator(&sentinel)));
	/// @brief Return readonly view with recursive iterators
	auto recursiveIterable() const noexcept
		-> decltype(std::ranges::subrange(_details::HierarchyIterator(first), _details::HierarchyIterator(&sentinel)));
};

} // namespace arch::hier
