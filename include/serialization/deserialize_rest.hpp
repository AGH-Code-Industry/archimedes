#pragma once

#include <serialization/concepts/deserializable.hpp>
#include <serialization/serialized.hpp>
#include <serialization/exception.hpp>
#include <typedesc.hpp>
#include <unordered_map>
#include <map>
#include <memory>
#include <iostream>

namespace arch::ser {
/// @brief Deserializing function.
/// @tparam S - Serializable type.
template<Deserializable S>
std::shared_ptr<S> deserialize_to(const Serialized& serialized) {
	if (serialized.type() != typedesc(S)) {
		throw arch::ser::SerializationException("type descriptors did not match");
	}
	std::shared_ptr<ISerializable> ptr = arch::ser::deserialize<S>(serialized);
	std::shared_ptr<S> to_return = std::dynamic_pointer_cast<S>(ptr);
	ptr.reset();
	return to_return;
}

template<Deserializable S>
std::shared_ptr<S> Serialized::deserialize_to() const {
	return arch::ser::deserialize_to<S>(*this);
}

/// @brief Helper class, intermediate in mapping and reading deserializers.
///
class deserializer_map {
public:
	/// @brief Type of pointer to deserializing function.
	///
	using deserializer_t = std::shared_ptr<ISerializable>(*)(const Serialized&);

	/// @brief Automaticly registers D's deserializing function.
	/// @brief Must by used only via REGISTER_DESERIALIZER() macro.
	/// @tparam D - type which deserializing function to register.
	template<Serializable S>
	static bool register_type();

	/// @brief Gets deserialzing function by type descriptor.
	/// @param td - typedesc of type.
	/// @return Pointer to deserializer, nullptr if type was not mapped.
	static deserializer_t get(const TypeDescriptorWrapper& td);

private:
	friend class Serialized;

	static inline std::unordered_map<TypeDescriptorWrapper, deserializer_t> _map;
};

template<Serializable S>
bool deserializer_map::register_type() {
	_map[typedesc(S)] = &arch::ser::deserialize<S>;
	return true;
}
}

/// @brief Registers type deserializing function.
///
#define REGISTER_DESERIALIZER(T) \
template<> \
struct arch::ser::registered_deserializing_fn<T> : std::bool_constant<true> { \
private: \
	static inline bool x = deserializer_map::register_type<T>(); \
};

/// @brief Default specialization of deserialize<T>(). Requires T to be DefaultConstructible.
/// @param T - type.
#define DEFAULT_DESERIALIZE(T) \
template<> \
std::shared_ptr<arch::ser::ISerializable> arch::ser::deserialize<T>(const arch::ser::Serialized& serialized) { \
	std::shared_ptr<arch::ser::ISerializable> to_return{new T}; \
	memcpy(to_return.get(), serialized._data, sizeof(T)); \
	return to_return; \
} \
REGISTER_DESERIALIZER(T)