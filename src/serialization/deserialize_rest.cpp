#include <serialization/deserialize_rest.hpp>

namespace arch::ser {
deserializer_map::deserializer_t deserializer_map::get(const TypeDescriptorWrapper& td) {
	auto found = _map.find(td);
	if (found == _map.end()) {
		return nullptr;
	}
	else {
		return found->second;
	}
}
}