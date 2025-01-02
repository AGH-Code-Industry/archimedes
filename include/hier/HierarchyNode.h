#pragma once

#include <ranges>

#include "ChildNode.h"
#include "ChildrenIterator.h"
#include "HierarchyIterator.h"
#include <ecs/ComponentTraits.h>
#include <utils/OptRef.h>

namespace arch::scene {
class Scene;
}

namespace arch::hier {

namespace _details {

class HierarchyIterator;
class ChildrenIterator;

} // namespace _details

/// @brief Main node class, implementing hierarchies
/// @details An in-place component
class HierarchyNode: public ChildNode {
	using Self = HierarchyNode;

public:

	/// @brief Makes HierarchyNode immovable in ecs storage, to preserve internal pointers
	static constexpr bool inPlaceComponent = true;

	/// @brief Entity constructor
	HierarchyNode(const ecs::Entity entity) noexcept;

	/// @brief Iterator type
	using Iterator = ChildrenIterator;
	/// @brief ReverseIterator type
	using ReverseIterator = std::reverse_iterator<ChildrenIterator>;

	bool setParent(Self& newParent) noexcept;
	bool isChildOf(const Self& other) const noexcept;

	Self& ancestor(u32 which = 1) noexcept;
	const Self& ancestor(u32 which = 1) const noexcept;
	i64 ancestry(const Self& other) const noexcept;

	Self& parent() noexcept;
	const Self& parent() const noexcept;

	Self& child(u32 which = 1) noexcept;
	const Self& child(u32 which = 1) const noexcept;
	OptRef<Self> childOpt(u32 which = 1) noexcept;
	OptRef<const Self> childOpt(u32 which = 1) const noexcept;
	Self& firstChild() noexcept;
	const Self& firstChild() const noexcept;
	OptRef<Self> firstChildOpt() noexcept;
	OptRef<const Self> firstChildOpt() const noexcept;
	Self& lastChild() noexcept;
	const Self& lastChild() const noexcept;
	OptRef<Self> lastChildOpt() noexcept;
	OptRef<const Self> lastChildOpt() const noexcept;
	u32 childrenCount() const noexcept;

	ecs::Entity entity() const noexcept;
	u32 depth() const noexcept;

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
		-> decltype(std::ranges::subrange(_details::HierarchyIterator(nullptr), _details::HierarchyIterator(nullptr)));
	/// @brief Return readonly view with recursive iterators
	auto recursiveIterable() const noexcept
		-> decltype(std::ranges::subrange(_details::HierarchyIterator(nullptr), _details::HierarchyIterator(nullptr)));

private:

	friend class scene::Scene;
	friend class _details::ChildrenIterator;
	friend class _details::HierarchyIterator;

	void _unparent() noexcept;
	void _setParentUnchecked(Self& newParent) noexcept;

	using ChildNode::_entity;
	using ChildNode::_next;
	using ChildNode::_prev;
	/// @brief Sentinel node, before first and after the last
	ChildNode _sentinel{ &_sentinel, &_sentinel, ecs::nullEntity };
	/// @brief First child node
	ChildNode* _first = nullptr;
	/// @brief Last child node
	ChildNode* _last = nullptr;
	/// @brief Children count
	u32 _count = 0;
	/// @brief Hierarchy depth (distance to root)
	u32 _depth = 0;
	/// @brief Parent node
	Self* _parent = nullptr;
};

} // namespace arch::hier
