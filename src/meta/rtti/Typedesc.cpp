#include <meta/rtti/Typedesc.h>

namespace arch::meta::rtti::_details {

template<>
const TypeDescriptor& ptrToTypeDesc<TypeDescriptor>(const TypeDescriptor* desc) noexcept {
	return *desc;
}

} // namespace arch::meta::rtti::_details
