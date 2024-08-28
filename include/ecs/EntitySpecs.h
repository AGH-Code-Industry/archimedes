#pragma once

#include <type_traits>

#include "ArchMath.h"
#include "Entity.h"

namespace arch::ecs {

template<class E, class V, size_t IdLen>
struct AutoEntitySpecs {
	using EntityT = E;

	using IdT = std::underlying_type_t<E>;
	using VersionT = V;

	static inline constexpr size_t idLength = IdLen;
};

template<class E>
struct EntitySpecs {
	static_assert("EntitySpecs not specialized for given type");
};

template<>
struct EntitySpecs<e32>: AutoEntitySpecs<e32, u8, 24> {};

template<>
struct EntitySpecs<e64>: AutoEntitySpecs<e64, u16, 48> {};

} // namespace arch::ecs
