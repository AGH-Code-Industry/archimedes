#pragma once

#include <functional>
#include <variant>

#include <archimedes/ecs/Entity.h>
#include <archimedes/scene/components/TransformComponent.h>
#include <archimedes/physics/collision_shapes/OBB.h>
#include <archimedes/physics/collision_shapes/Triangle.h>
#include <archimedes/physics/collision_shapes/Circle.h>
#include <archimedes/physics/collision_shapes/HorizontalLine.h>
#include <archimedes/physics/collision_shapes/VerticalLine.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

struct ColliderComponent {
	std::variant<OBB, Triangle, Circle, HorizontalLine, VerticalLine> shape;
	std::function<void(ecs::Entity, ecs::Entity)> action;

	static bool areColliding(const ColliderComponent& collider1, const ColliderComponent& collider2,
		const TransformComponent& transform1, const TransformComponent& transform2);
};

} // namespace arch::physics
