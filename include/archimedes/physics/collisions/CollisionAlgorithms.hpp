#pragma once
#include <concepts>

#include <archimedes/physics/collisions/ColliderComponent.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

/// @brief Check mouse to shape collision
/// (optimised by static polymorphism instead of virtual functions)
template<std::derived_from<Shape> ShapeA>
bool checkPoint(const ShapeA& shape, const TransformComponent& transform, float2 mousePosition) {
	return shape.containsPoint(transform, mousePosition);
}

/// @brief If a collision exists, all projections need to overlap
bool areProjectionsOverlapping(float2 projection1, float2 projection2);

/// @brief Get overlap length (used for calculating collision depth)
f32 getOverlap(float2 projection1, float2 projection2);

/// @brief SAT (Separate Axis Test) algorithm for checking collision between convex shapes
/// (optimised by static polymorphism instead of virtual functions)
template<
	std::derived_from<Shape> ShapeA,
	std::derived_from<Shape> ShapeB,
	std::size_t ArraySize1,
	std::size_t ArraySize2>
std::optional<Collision> checkSAT(
	const std::array<float2, ArraySize1>& axes1,
	const std::array<float2, ArraySize2>& axes2,
	const TransformComponent& transform1,
	const TransformComponent& transform2,
	const ShapeA& shape1,
	const ShapeB& shape2
) {
	float2 normal(0.0f);
	f32 depth = std::numeric_limits<f32>::max();
	for (auto& axis : axes1) {
		float2 projection1 = shape1.getProjection(transform1, axis);
		float2 projection2 = shape2.getProjection(transform2, axis);
		if (!areProjectionsOverlapping(projection1, projection2)) {
			return std::nullopt;
		}
		f32 overlap = getOverlap(projection1, projection2);
		if (overlap < depth) {
			depth = overlap;
			normal = axis;
		}
	}
	for (auto& axis : axes2) {
		float2 projection1 = shape1.getProjection(transform1, axis);
		float2 projection2 = shape2.getProjection(transform2, axis);
		if (!areProjectionsOverlapping(projection1, projection2)) {
			return std::nullopt;
		}
		f32 overlap = getOverlap(projection1, projection2);
		if (overlap < depth) {
			depth = overlap;
			normal = axis;
		}
	}
	return Collision(normal, depth, CollisionState::CurrentlyFound);
}

/// @brief If there doesn't exist a function to check for collision between two shapes,
/// swap the shapes and use an existing function

template<typename ShapeA, typename ShapeB>
concept CollidableReversed = requires(
	const ShapeB& shape1,
	const ShapeA& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	{ checkCollision(shape1, shape2, transform1, transform2) } -> std::same_as<std::optional<Collision>>;
};

template<typename ShapeA, typename ShapeB>
requires CollidableReversed<ShapeA, ShapeB>
std::optional<Collision> checkCollision(
	const ShapeA& shape1,
	const ShapeB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	return checkCollision(shape2, shape1, transform2, transform1);
}

/// @brief All algorithms for shape-shape collisions

std::optional<Collision> checkCollision(
	const OBB& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

std::optional<Collision> checkCollision(
	const Triangle& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

std::optional<Collision> checkCollision(
	const Triangle& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

std::optional<Collision> checkCollision(
	const Circle& shape1,
	const Circle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

std::optional<Collision> checkCollision(
	const Circle& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

std::optional<Collision> checkCollision(
	const Circle& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

} // namespace arch::physics
