#pragma once

#include <functional>
#include <variant>
#include <bitset>
#include <optional>

#include <archimedes/ecs/Entity.h>
#include <archimedes/physics/collisions/collision_shapes/Circle.h>
#include <archimedes/physics/collisions/collision_shapes/OBB.h>
#include <archimedes/physics/collisions/collision_shapes/Triangle.h>
#include <archimedes/physics/collisions/Collision.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

///@brief Lets you to find collisions between entities
struct ColliderComponent {
	std::variant<OBB, Triangle, Circle> shape;

	/// @brief If true, the object can collide with mouse pointer
	bool detectsMouse = false;

	/// @brief A bitmask that tells on which layers the entity can be detected by other colliders
	/// Each bit represents another layer, set it to 1 to make the object detectable on it
	std::bitset<32> isScannedMask{1};

	/// @brief A bitmask that tells which layers the entity can detect.
	/// If you set 1 for a layer, the object will detect collisions with entities on this layer
	std::bitset<32> scansMask{1};

	/// @brief Collision checking functions
	
	static std::optional<Collision> areColliding(
		const ColliderComponent& collider1,
		const ColliderComponent& collider2,
		const TransformComponent& transform1,
		const TransformComponent& transform2
	);

	static bool isCollidingWithMouse(
		const ColliderComponent& collider,
		const TransformComponent& transform,
		float2 mousePosition
	);
};

} // namespace arch::physics
