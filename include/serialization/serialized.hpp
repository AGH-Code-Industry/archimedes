#pragma once

#include <serialization/concepts/serializable.hpp>
#include <serialization/concepts/deserializable.hpp>
#include <serialization/deserialize.hpp>
#include <typedesc.hpp>
#include <memory>

namespace arch::ser {
/// @brief Class of serialized object.
///
class Serialized {
public:
	/// @brief Destructor. Frees stored data.
	///
	~Serialized();

	/// @brief Deserializing function.
	///
	std::shared_ptr<ISerializable> deserialize() const;
	/// @brief Deserializing function.
	/// @brief Compares stored type descriptor with one of given type, throws on inequality.
	template<Deserializable S>
	std::shared_ptr<S> deserialize_to() const;

	/// @brief Returns type descriptor.
	///
	const TypeDescriptorWrapper& type() const;

	/// @brief Creates new Serialized object with given data.
	/// @tparam S - type of object to be serialized.
	/// @param data - serialized data.
	template<class S>
	static Serialized new_serialized(void* data);

private:
	template<Serializable S>
	friend std::shared_ptr<ISerializable> deserialize(const Serialized&);

	friend class ISerializable;

	Serialized(const TypeDescriptorWrapper& td, void* d);

	void* _data;
	const TypeDescriptorWrapper& _typedesc;
};

template<class S>
static Serialized Serialized::new_serialized(void* data) {
	return Serialized(typedesc(S), data);
}
}