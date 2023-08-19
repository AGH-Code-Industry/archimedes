#pragma once

#include <concepts>
#include <serialization/deserialize.hpp>

namespace arch::ser {
/// @brief Concept satisfied only if there exist specialization of deserialize() function for T, and T's deserializer was registered.
///
template<class T>
concept Deserializable = registered_deserializing_fn<T>::value and requires(Serialized a) {
	{ deserialize<T>(a) } -> std::same_as<std::shared_ptr<ISerializable>>;
};
}