#include <physics/Collisions.hpp>

#include <physics/ColliderComponent.h>
#include <physics/PhysicsException.h>

namespace arch::physics {
bool ColliderComponent::areColliding(const ColliderComponent& lhs, const ColliderComponent& rhs) {
	return std::visit([](const auto& a, const auto& b) -> bool {
		return checkCollision(a, b);
	}, lhs.shape, rhs.shape);
}
}

