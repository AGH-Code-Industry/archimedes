#pragma once
#include <archimedes/physics/ColliderComponent.h>

namespace arch::physics {

	template<typename T1, typename T2>
	concept CollidableReversed = requires(T2 a, T1 b) {
		{ checkCollision(a, b) } -> std::same_as<bool>;
	};

	template<typename T1, typename T2>
	requires CollidableReversed<T1, T2>
	bool checkCollision(T1 a, T2 b) {
		return checkCollision(b, a);
	}

	bool checkCollision(const AABB& lhs, const AABB& rhs);
} // namespace arch::physics