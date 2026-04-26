#pragma once
#include <archimedes/physics/collisions/ColliderComponent.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

/// @brief If a collision exists, all projections need to overlap
bool areProjectionsOverlapping(float2 projection1, float2 projection2);

/// @brief Get overlap length (used for calculating collision depth)
f32 getOverlap(float2 projection1, float2 projection2);

/// @brief Horizontal line - polygon collision checking algorithm
std::optional<Collision> checkHorizontalLineAndPolygon(f32 y, const std::vector<float3>& vertices);

/// @brief Vertical line - polygon collision checking algorithm
std::optional<Collision> checkVerticalLineAndPolygon(f32 x, const std::vector<float3>& vertices);

/// @brief SAT (Separate Axis Test) algorithm for checking collision between convex shapes
std::optional<Collision> checkSAT(
	const std::vector<float3>& axes1,
	const std::vector<float3>& axes2,
	const TransformComponent& transform1,
	const TransformComponent& transform2,
	const Shape& shape1,
	const Shape& shape2
);

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
