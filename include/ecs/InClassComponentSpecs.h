#pragma once

#include <bit>
#include <concepts>

namespace arch::ecs::_details { // NOLINT

/// @brief Concept modeled by types flaged as 'InPlaceComponent' in their definition
template<class C>
concept PascalCaseInClassInPlaceComponent = requires { typename C::InPlaceComponent; } || C::InPlaceComponent;

/// @brief Concept modeled by types flaged as 'inPlaceComponent' in their definition
template<class C>
concept CamelCaseInClassInPlaceComponent = requires { typename C::inPlaceComponent; } || C::inPlaceComponent;

/// @brief Concept modeled by types flaged as 'in_place_component' in their definition
template<class C>
concept SnakeCaseInClassInPlaceComponent = requires { typename C::in_place_component; } || C::in_place_component;

/// @brief Concept modeled by types flaged as 'in place component' in their definition
template<class C>
concept AnyInClassInPlaceComponent =
	PascalCaseInClassInPlaceComponent<C> || CamelCaseInClassInPlaceComponent<C> || SnakeCaseInClassInPlaceComponent<C>;

/// @brief Concept modeled by types that specify 'ComponentPageSize' in their definition
template<class C>
concept PascalCaseInClassComponentPageSize = requires {
	{ C::ComponentPageSize } -> std::convertible_to<size_t>;
};

/// @brief Concept modeled by types that specify 'componentPageSize' in their definition
template<class C>
concept CamelCaseInClassComponentPageSize = requires {
	{ C::componentPageSize } -> std::convertible_to<size_t>;
};

/// @brief Concept modeled by types that specify 'component_page_size' in their definition
template<class C>
concept SnakeCaseInClassComponentPageSize = requires {
	{ C::component_page_size } -> std::convertible_to<size_t>;
};

/// @brief Concept modeled by types that specify 'component page size' in their definition
template<class C>
concept AnyInClassComponentPageSize = PascalCaseInClassComponentPageSize<C> || CamelCaseInClassComponentPageSize<C> ||
	SnakeCaseInClassComponentPageSize<C>;

/// @brief Provides member constant value for types without page size specification in their definition
/// @tparam C - component type
template<class C>
struct InClassComponentPageSizeValue {
	static inline constexpr size_t value = 0;
};

/// @brief Provides member constant value for types with PascalCase page size specification in their definition
/// @tparam C - component type
template<PascalCaseInClassComponentPageSize C>
struct InClassComponentPageSizeValue<C> {
	static inline constexpr size_t value = C::ComponentPageSize;
};

/// @brief Provides member constant value for types with camelCase page size specification in their definition
/// @tparam C - component type
template<CamelCaseInClassComponentPageSize C>
struct InClassComponentPageSizeValue<C> {
	static inline constexpr size_t value = C::componentPageSize;
};

/// @brief Provides member constant value for types with snake_case page size specification in their definition
/// @tparam C - component type
template<SnakeCaseInClassComponentPageSize C>
struct InClassComponentPageSizeValue<C> {
	static inline constexpr size_t value = C::component_page_size;
};

} // namespace arch::ecs::_details
