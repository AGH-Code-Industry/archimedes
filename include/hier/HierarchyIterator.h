#pragma once

#include <ecs/Entity.h>

namespace arch::hier {

class HierarchyNode;

/// @brief Simple hierarchy iterator, providing access to children of an entity
/// @details Models std::bidirectional_iterator
class HierarchyIterator {
public:

	HierarchyIterator(const ChildNode* node) noexcept;
	HierarchyIterator() noexcept = default;
	HierarchyIterator(const HierarchyIterator&) noexcept = default;
	HierarchyIterator(HierarchyIterator&&) noexcept = default;

	HierarchyIterator& operator=(const HierarchyIterator&) noexcept = default;
	HierarchyIterator& operator=(HierarchyIterator&&) noexcept = default;

	bool operator==(const HierarchyIterator&) const noexcept = default;

	ecs::Entity operator*() const noexcept;

	HierarchyIterator operator++(int) noexcept;
	HierarchyIterator& operator++() noexcept;

	HierarchyIterator operator--(int) noexcept;
	HierarchyIterator& operator--() noexcept;

private:

	const ChildNode* _current;
};

} // namespace arch::hier

template<>
struct std::iterator_traits<arch::hier::HierarchyIterator> {
	using difference_type = std::ptrdiff_t;
	using value_type = arch::ecs::Entity;
	using pointer = void;
	using reference = void;
	using iterator_category = std::bidirectional_iterator_tag;
};
