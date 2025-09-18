#pragma once
#include <archimedes/physics/components/ColliderComponent.h>
#include <archimedes/scene/components/TransformComponent.h>


namespace arch::physics {

	using TransformComponent = scene::components::TransformComponent;

	template<typename ShapeA, typename ShapeB>
	concept CollidableReversed = requires(const ShapeB& shape1, const ShapeA& shape2,
		const TransformComponent& transform1, const TransformComponent& transform2) {
		{ checkCollision(shape1, shape2, transform1, transform2) } -> std::same_as<bool>;
	};

	template<typename ShapeA, typename ShapeB>
	requires CollidableReversed<ShapeA, ShapeB>
	bool checkCollision(const ShapeA& shape1, const ShapeB& shape2, const TransformComponent& transform1,
		const TransformComponent& transform2) {
		return checkCollision(shape2, shape1, transform2, transform1);
	}

	bool checkCollision(const AABB& shape1, const AABB& shape2, const TransformComponent& transform1,
		const TransformComponent& transform2);
} // namespace arch::physics