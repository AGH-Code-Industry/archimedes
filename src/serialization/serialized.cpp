#include <serialization/serialized.hpp>
#include <serialization/exception.hpp>
#include <serialization/deserialize_rest.hpp>

namespace arch::ser {
Serialized::Serialized(const TypeDescriptorWrapper& td, void* d) :
	_data{d},
	_typedesc{td} {}
Serialized::~Serialized() {
	free(_data);
}
std::shared_ptr<ISerializable> Serialized::deserialize() const {
	auto found = deserializer_map::_map.find(_typedesc);
	if (found == deserializer_map::_map.end()) {
		throw SerializationException('\'' + _typedesc.name() + "' was not registered with deserializer");
	}

	return found->second(*this);
}
const TypeDescriptorWrapper& Serialized::type() const {
	return _typedesc;
}
}