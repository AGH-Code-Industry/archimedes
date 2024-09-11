#pragma once

#include <bit>
#include <concepts>

namespace arch::ecs::_details { // NOLINT

/// @brief Concept satisfied by types flaged as 'InPlaceComponent' in their definition
template<class C>
concept PascalCaseInclassInPlaceComponent = requires { typename C::InPlaceComponent; } or C::InPlaceComponent;

/// @brief Concept satisfied by types flaged as 'inPlaceComponent' in their definition
template<class C>
concept CamelCaseInclassInPlaceComponent = requires { typename C::inPlaceComponent; } or C::inPlaceComponent;

/// @brief Concept satisfied by types flaged as 'in_place_component' in their definition
template<class C>
concept SnakeCaseInclassInPlaceComponent = requires { typename C::in_place_component; } or C::in_place_component;

/// @brief Concept satisfied by types flaged as 'in place component' in their definition
template<class C>
concept AnyInclassInPlaceComponent =
	PascalCaseInclassInPlaceComponent<C> or CamelCaseInclassInPlaceComponent<C> or SnakeCaseInclassInPlaceComponent<C>;

/// @brief Concept satisfied by types that specify 'ComponentPageSize' in their definition
template<class C>
concept PascalCaseInclassComponentPageSize = requires {
												 { C::ComponentPageSize } -> std::convertible_to<size_t>;
											 };

/// @brief Concept satisfied by types that specify 'componentPageSize' in their definition
template<class C>
concept CamelCaseInclassComponentPageSize = requires {
												{ C::componentPageSize } -> std::convertible_to<size_t>;
											};

/// @brief Concept satisfied by types that specify 'component_page_size' in their definition
template<class C>
concept SnakeCaseInclassComponentPageSize = requires {
												{ C::component_page_size } -> std::convertible_to<size_t>;
											};

/// @brief Concept satisfied by types that specify 'component page size' in their definition
template<class C>
concept AnyInclassComponentPageSize = PascalCaseInclassComponentPageSize<C> or CamelCaseInclassComponentPageSize<C> or
	SnakeCaseInclassComponentPageSize<C>;

/// @brief Provides member constant value for types without page size specification in their definition
/// @tparam C - component type
template<class C>
struct InclassComponentPageSizeValue {
	static inline constexpr size_t value = 0;
};

/// @brief Provides member constant value for types with PascalCase page size specification in their definition
/// @tparam C - component type
template<PascalCaseInclassComponentPageSize C>
struct InclassComponentPageSizeValue<C> {
	static inline constexpr size_t value = C::ComponentPageSize;
};

/// @brief Provides member constant value for types with camelCase page size specification in their definition
/// @tparam C - component type
template<CamelCaseInclassComponentPageSize C>
struct InclassComponentPageSizeValue<C> {
	static inline constexpr size_t value = C::componentPageSize;
};

/// @brief Provides member constant value for types with snake_case page size specification in their definition
/// @tparam C - component type
template<SnakeCaseInclassComponentPageSize C>
struct InclassComponentPageSizeValue<C> {
	static inline constexpr size_t value = C::component_page_size;
};

} // namespace arch::ecs::_details
