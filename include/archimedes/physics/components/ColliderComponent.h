#pragma once

#include <functional>
#include <variant>

#include <archimedes/ecs/Entity.h>
#include <archimedes/physics/collision_shapes/OBB.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

struct ColliderComponent {
	std::variant<OBB> shape;
	std::function<void(ecs::Entity, ecs::Entity)> action;

	static bool areColliding(const ColliderComponent& collider1, const ColliderComponent& collider2,
		const TransformComponent& transform1, const TransformComponent& transform2);
};

} // namespace arch::physics
