#include <meta/rtti/Typedesc.h>

namespace arch::meta::rtti::_details {

Helper Helper::operator*() const noexcept {
	return const_cast<Helper&>(*this);
}

} // namespace arch::meta::rtti::_details
