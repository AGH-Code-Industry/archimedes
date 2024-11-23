#pragma once

#include <concepts>
#include <meta/thisT/UniqueGetter.hpp>

namespace arch::meta::rtti {

/// @brief Concept that checks whether type is usable in Archimedes RTTI system
/// @brief Non-polymorphic types model RTIIEnabled
template<class T>
concept RTTIEnabled =
	(std::is_polymorphic_v<T> ? std::same_as<
									typename T::template _ARCH_RTTI_UNIQUE<T>::Unique,
									typename T::template _ARCH_RTTI_UNIQUE<T>::Unique> &&
			 std::same_as<typename arch::meta::thisT::UniqueGetter<T>::type,
						  typename T::template _ARCH_RTTI_UNIQUE<T>::Unique> :
								true);

} // namespace arch::meta::rtti
