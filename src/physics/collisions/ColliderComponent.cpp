#include <archimedes/physics/collisions/CollisionAlgorithms.hpp>
#include <archimedes/physics/collisions/ColliderComponent.h>
#include <archimedes/physics/collisions/PointAlgorithms.h>

namespace arch::physics {
bool ColliderComponent::areColliding(
	const ColliderComponent& collider1,
	const ColliderComponent& collider2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	if((collider1.scansMask & collider2.isScannedMask).none()) {
		return false;
	}
	return std::visit(
		[transform1, transform2](const auto& a, const auto& b) -> bool {
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
	if (!collider.detectsMouse) {
		return false;
	}
	return std::visit(
		[transform, mousePosition](const auto& shape) -> bool {
			return containsPoint(shape, mousePosition, transform);
		},
		collider.shape
	);
}
} // namespace arch::physics

