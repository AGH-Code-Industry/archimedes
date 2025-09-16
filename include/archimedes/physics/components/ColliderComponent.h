#pragma once

#include <functional>
#include <variant>

#include <archimedes/physics/collision_shapes/AABB.h>
#include <archimedes/ecs/Entity.h>

namespace arch::physics {

enum ShapeType {
	aabb
};

struct ColliderComponent {
	ShapeType type;
	std::variant<AABB> shape;
	std::function<void(ecs::Entity, ecs::Entity)> action;

	static bool areColliding(const ColliderComponent& lhs, const ColliderComponent& rhs);
};

} // namespace arch::physics
