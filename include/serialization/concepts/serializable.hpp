#pragma once

#include <concepts>
#include <memory>

namespace arch::ser {
class Serialized;
class ISerializable;

/// @brief Concept satisfied only if T derives from ISerializable and overrides serialize() method.
///
template<class T>
concept Serializable = std::derived_from<T, ISerializable> and not std::is_abstract_v<T> and requires(T a) {
	{ a.serialize() } -> std::same_as<Serialized>;
};
}