#include <hier/Siblings.h>

namespace arch::hier {

ChildNode& Siblings::node() noexcept {
	return *reinterpret_cast<ChildNode*>(this);
}

const ChildNode& Siblings::node() const noexcept {
	return *reinterpret_cast<const ChildNode*>(this);
}

} // namespace arch::hier
