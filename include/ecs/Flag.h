#pragma once

namespace arch::ecs::_details { // NOLINT

/// @brief Checks if component was marked as flag-component
template<class T>
concept FlagComponent = T::flag_component or T::flagComponent or T::FlagComponent;

} // namespace arch::ecs::_details
