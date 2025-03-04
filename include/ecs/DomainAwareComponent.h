#pragma once

#include <concepts>

#include "Entity.h"

namespace arch::ecs {

class Domain;

template<class T, class... Args>
concept DomainAwareComponent = std::constructible_from<T, Domain&, const Entity, Args...>;

} // namespace arch::ecs
