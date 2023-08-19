#pragma once

#include "serializable.hpp"
#include "deserializable.hpp"

namespace arch::ser {
/// @brief Concept satisfied only if T is Serializable and Deserializable.
///
template<class T>
concept FullySerializable = Serializable<T> and Deserializable<T>;
}