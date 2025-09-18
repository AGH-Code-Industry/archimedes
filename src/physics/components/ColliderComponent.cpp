#include <archimedes/physics/Collisions.hpp>
#include <archimedes/physics/components/ColliderComponent.h>

namespace arch::physics {
bool ColliderComponent::areColliding(const ColliderComponent& collider1, const ColliderComponent& collider2,
	const TransformComponent& transform1, const TransformComponent& transform2) {
	return std::visit(
	[transform1, transform2](const auto& a, const auto& b) -> bool {
			return checkCollision(a, b, transform1, transform2);
		},
		collider1.shape,
		collider2.shape
	);
}
}

