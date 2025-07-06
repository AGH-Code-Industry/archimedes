#pragma once
#include <physics/ColliderComponent.h>

namespace arch::physics {
	bool checkCollision(const AABB& lhs, const AABB& rhs);
} // namespace arch::physics