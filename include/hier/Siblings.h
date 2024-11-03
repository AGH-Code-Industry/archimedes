#pragma once

namespace arch::hier {

struct ChildNode;

struct Siblings {
	const Siblings* prev;
	const Siblings* next;

	ChildNode& node() noexcept;
	const ChildNode& node() const noexcept;
};

} // namespace arch::hier
