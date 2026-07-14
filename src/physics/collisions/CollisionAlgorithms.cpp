#include <archimedes/physics/collisions/CollisionAlgorithms.hpp>

#include <archimedes/physics/Helpers.hpp>

namespace arch::physics {

bool areProjectionsOverlapping(float2 projection1, float2 projection2) {
	return !(projection1.y < projection2.x || projection2.y < projection1.x);
}

f32 getOverlap(float2 projection1, float2 projection2) {
	f32 maxX = projection1.x > projection2.x ? projection1.x : projection2.x;
	f32 minY = projection1.y < projection2.y ? projection1.y : projection2.y;
	return std::max(0.0f, minY - maxX);
}

std::optional<Collision> checkCollision(
	const OBB& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	std::array<float2, 2> axes1 = shape1.getSeparatingAxes(transform1);
	std::array<float2, 2> axes2 = shape2.getSeparatingAxes(transform2);
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

std::optional<Collision> checkCollision(
	const Triangle& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	std::array<float2, 3> axes1 = shape1.getSeparatingAxes(transform1);
	std::array<float2, 3> axes2 = shape2.getSeparatingAxes(transform2);
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

std::optional<Collision> checkCollision(
	const Triangle& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	std::array<float2, 3> axes1 = shape1.getSeparatingAxes(transform1);
	std::array<float2, 2> axes2 = shape2.getSeparatingAxes(transform2);
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

std::optional<Collision> checkCollision(
	const Circle& shape1,
	const Circle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	float2 center1 = shape1.getRealCenter(transform1);
	float2 center2 = shape2.getRealCenter(transform2);
	f32 distance = glm::distance(center1, center2);
	f32 radius1 = shape1.getRealRadius(transform1);
	f32 radius2 = shape2.getRealRadius(transform2);
	f32 radiusSum = radius1 + radius2;
	if (distance > radiusSum + COLLISIONS_EPSILON) {
		return std::nullopt;
	}
	f32 depth = radiusSum - distance;
	float2 normal = glm::normalize(center2 - center1);
	return Collision(normal, depth, CollisionState::CurrentlyFound);
}

std::optional<Collision> checkCollision(
	const Circle& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	// a circle has just one, special axis
	std::array<float2, 3> axes2 = shape2.getSeparatingAxes(transform2);
	float2 axis1 = shape1.getSeparatingAxis(transform1, shape2.getRealVertices(transform2));
	std::array<float2, 1> axes1 = {axis1};
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

std::optional<Collision> checkCollision(
	const Circle& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	// a circle has just one, special axis
	std::array<float2, 2> axes2 = shape2.getSeparatingAxes(transform2);
	float2 axis1 = shape1.getSeparatingAxis(transform1, shape2.getRealVertices(transform2));
	std::array<float2, 1> axes1 = {axis1};
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

} // namespace arch::physics
