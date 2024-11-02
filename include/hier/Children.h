#pragma once

#include <vector>

#include "ChildNode.h"
#include "ChildrenIterator.h"
#include <ecs/Entity.h>

namespace arch::hier {

/// @brief Children component, in form of 'linked' list
/// @details Models std::ranges::bidirectional_range
struct Children {
	using Iterator = ChildrenIterator;
	using ReverseIterator = std::reverse_iterator<ChildrenIterator>;

	ChildNode beginSentinel{ ecs::nullEntity, nullptr, &endSentinel };
	ChildNode endSentinel{ ecs::nullEntity, &beginSentinel, nullptr };
	size_t count = 0;
	ChildNode* first = &endSentinel;
	ChildNode* last = &endSentinel;

	Iterator begin() const noexcept;
	Iterator cbegin() const noexcept;
	Iterator end() const noexcept;
	Iterator cend() const noexcept;
	ReverseIterator rbegin() const noexcept;
	ReverseIterator crbegin() const noexcept;
	ReverseIterator rend() const noexcept;
	ReverseIterator crend() const noexcept;
};

} // namespace arch::hier
