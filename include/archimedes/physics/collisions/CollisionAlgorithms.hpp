#pragma once
#include <archimedes/physics/collisions/ColliderComponent.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {
using TransformComponent = scene::components::TransformComponent;

bool areProjectionsOverlapping(float2 projection1, float2 projection2);

bool checkSAT(
	const std::vector<float3>& axes1,
	const std::vector<float3>& axes2,
	const TransformComponent& transform1,
	const TransformComponent& transform2,
	const SATShape& shape1,
	const SATShape& shape2
);

template<typename ShapeA, typename ShapeB>
concept CollidableReversed = requires(
	const ShapeB& shape1,
	const ShapeA& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	{ checkCollision(shape1, shape2, transform1, transform2) } -> std::same_as<bool>;
};

template<typename ShapeA, typename ShapeB>
requires CollidableReversed<ShapeA, ShapeB>
bool checkCollision(
	const ShapeA& shape1,
	const ShapeB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	return checkCollision(shape2, shape1, transform2, transform1);
}

bool checkCollision(
	const OBB& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const Triangle& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const Triangle& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const Circle& shape1,
	const Circle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const Circle& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const Circle& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const HorizontalLine& shape1,
	const HorizontalLine& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const VerticalLine& shape1,
	const VerticalLine& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const HorizontalLine& shape1,
	const VerticalLine& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const HorizontalLine& shape1,
	const Circle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const HorizontalLine& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const HorizontalLine& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const VerticalLine& shape1,
	const Circle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const VerticalLine& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

bool checkCollision(
	const VerticalLine& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
);

} // namespace arch::physics
