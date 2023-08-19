#pragma once

#include "concepts/serializable.hpp"

namespace arch::ser {
/// @brief Deserializing function, must be specialized.
/// @tparam S - Serializable type
template<Serializable S>
std::shared_ptr<ISerializable> deserialize(const Serialized&) = delete;

/// @brief Helper struct holding info if type's deserializer was registered using REGISTER_DESERIALIZER()
/// @tparam S - type
template<Serializable S>
struct registered_deserializing_fn : std::bool_constant<false> {};
}