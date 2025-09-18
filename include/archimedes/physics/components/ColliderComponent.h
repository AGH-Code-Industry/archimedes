#pragma once

#include <functional>
#include <variant>

#include <archimedes/scene/components/TransformComponent.h>
#include <archimedes/ecs/Entity.h>
#include <archimedes/physics/collision_shapes/AABB.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

struct ColliderComponent {
	std::variant<AABB> shape;
	std::function<void(ecs::Entity, ecs::Entity)> action;

	static bool areColliding(const ColliderComponent& collider1, const ColliderComponent& collider2,
		const TransformComponent& transform1, const TransformComponent& transform2);
};

} // namespace arch::physics
