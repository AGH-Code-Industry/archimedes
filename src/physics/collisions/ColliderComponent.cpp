#include <archimedes/physics/collisions/CollisionAlgorithms.hpp>
#include <archimedes/physics/collisions/ColliderComponent.h>

namespace arch::physics {
std::optional<Collision> ColliderComponent::areColliding(
	const ColliderComponent& collider1,
	const ColliderComponent& collider2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	// you have to resolve a std::variant variable's type
	return std::visit(
		[&](const auto& a, const auto& b) -> std::optional<Collision> {
			return checkCollision(a, b, transform1, transform2);
		},
		collider1.shape,
		collider2.shape
	);
}

bool ColliderComponent::isCollidingWithMouse(
	const ColliderComponent& collider,
	const TransformComponent& transform,
	float3 mousePosition
) {
	return std::visit(
		[&](const auto& shape) -> bool {
			return checkPoint(shape, transform, mousePosition);
		},
		collider.shape
	);
}
} // namespace arch::physics

