#include <archimedes/physics/collisions/CollisionAlgorithms.hpp>

namespace arch::physics {

bool areProjectionsOverlapping(float2 projection1, float2 projection2) {
	return !(projection1.y < projection2.x || projection2.y < projection1.x);
}

f32 getOverlap(float2 projection1, float2 projection2) {
	f32 maxX = projection1.x > projection2.x ? projection1.x : projection2.x;
	f32 minY = projection1.y < projection2.y ? projection1.y : projection2.y;
	return std::max(0.0f, minY - maxX);
}

std::optional<Collision> checkHorizontalLineAndPolygon(f32 y, const std::vector<float3>& vertices) {
	bool aboveFound = false;
	bool belowFound = false;
	f32 minY = std::numeric_limits<f32>::max();
	f32 maxY = std::numeric_limits<f32>::min();
	for (const auto& vertex : vertices) {
		minY = std::min(minY, vertex.y);
		maxY = std::max(maxY, vertex.y);
		if (vertex.y > y) {
			aboveFound = true;
		} else if (vertex.y < y) {
			belowFound = true;
		}
		if (aboveFound && belowFound) {
			// choose the closest vertex to the line
			f32 depth = std::min(maxY - y, y - minY);
			float3 normal = { 0.0f, 1.0f, 0.0f };
			return Collision(normal, depth, CollisionState::CurrentlyFound);
			}
		}
	return std::nullopt;
}

std::optional<Collision> checkVerticalLineAndPolygon(f32 x, const std::vector<float3>& vertices) {
	bool leftFound = false;
	bool rightFound = false;
	f32 minX = std::numeric_limits<f32>::max();
	f32 maxX = std::numeric_limits<f32>::min();
	for (const auto& vertex : vertices) {
		minX = std::min(minX, vertex.x);
		maxX = std::max(maxX, vertex.x);
		if (vertex.x > x) {
			rightFound = true;
		} else if (vertex.x < x) {
			leftFound = true;
		}
		if (leftFound && rightFound) {
			// choose the closest vertex to the line
			f32 depth = std::min(maxX - x, x - minX);
			float3 normal = { 1.0f, 0.0f, 0.0f };
			return Collision(normal, depth, CollisionState::CurrentlyFound);
			}
		}
	return std::nullopt;
}

std::optional<Collision> checkSAT(
	const std::vector<float3>& axes1,
	const std::vector<float3>& axes2,
	const TransformComponent& transform1,
	const TransformComponent& transform2,
	const SATShape& shape1,
	const SATShape& shape2
) {
	float3 normal(0.0f);
	f32 depth = std::numeric_limits<f32>::max();
	for (auto& axis : axes1) {
		float2 projection1 = shape1.getProjection(axis, transform1);
		float2 projection2 = shape2.getProjection(axis, transform2);
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
		float2 projection1 = shape1.getProjection(axis, transform1);
		float2 projection2 = shape2.getProjection(axis, transform2);
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

std::optional<Collision> checkCollision(
	const OBB& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	std::vector<float3> axes1 = shape1.getSeparatingAxes(transform1);
	std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

std::optional<Collision> checkCollision(
	const Triangle& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	std::vector<float3> axes1 = shape1.getSeparatingAxes(transform1);
	std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

std::optional<Collision> checkCollision(
	const Triangle& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	std::vector<float3> axes1 = shape1.getSeparatingAxes(transform1);
	std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

std::optional<Collision> checkCollision(
	const Circle& shape1,
	const Circle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	float3 center1 = shape1.getRealCenter(transform1);
	float3 center2 = shape2.getRealCenter(transform2);
	f32 distance = glm::distance(center1, center2);
	f32 radius1 = shape1.getRealRadius(transform1);
	f32 radius2 = shape2.getRealRadius(transform2);
	f32 radiusSum = radius1 + radius2;
	if (distance > radiusSum + 0.0001f) {
		return std::nullopt;
	}
	f32 depth = radiusSum - distance;
	float3 normal = glm::normalize(center2 - center1);
	return Collision(normal, depth, CollisionState::CurrentlyFound);
}

std::optional<Collision> checkCollision(
	const Circle& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	// a circle has just one, special axis
	std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
	float3 axis1 = shape1.getSeparatingAxis(transform1, shape2.getRealVertices(transform2));
	std::vector<float3> axes1 = { axis1 };
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

std::optional<Collision> checkCollision(
	const Circle& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	// a circle has just one, special axis
	std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
	float3 axis1 = shape1.getSeparatingAxis(transform1, shape2.getRealVertices(transform2));
	std::vector<float3> axes1 = { axis1 };
	return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
}

// Lines don't collide with each other (otherwise they would either always or almost never do it)
std::optional<Collision> checkCollision(
	const HorizontalLine& shape1,
	const HorizontalLine& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	return std::nullopt;
}

std::optional<Collision> checkCollision(
	const VerticalLine& shape1,
	const VerticalLine& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	return std::nullopt;
}

std::optional<Collision> checkCollision(
	const HorizontalLine& shape1,
	const VerticalLine& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	return std::nullopt;
}

std::optional<Collision> checkCollision(
	const HorizontalLine& shape1,
	const Circle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	f32 y = shape1.getRealPosition(transform1);
	float3 center = shape2.getRealCenter(transform2);
	f32 radius = shape2.getRealRadius(transform2);
	f32 difference = center.y - y;
	f32 distance = std::abs(difference);
	if (distance > radius + 0.0001f) {
		return std::nullopt;
	}
	float3 normal = { 0.0f, 1.0f, 0.0f };
	f32 depth = radius - distance;
	return Collision(normal, depth, CollisionState::CurrentlyFound);
}

std::optional<Collision> checkCollision(
	const HorizontalLine& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	f32 y = shape1.getRealPosition(transform1);
	std::vector<float3> vertices = shape2.getRealVertices(transform2);
	return checkHorizontalLineAndPolygon(y, vertices);
}

std::optional<Collision> checkCollision(
	const HorizontalLine& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	f32 y = shape1.getRealPosition(transform1);
	std::vector<float3> vertices = shape2.getRealVertices(transform2);
	return checkHorizontalLineAndPolygon(y, vertices);
}

std::optional<Collision> checkCollision(
	const VerticalLine& shape1,
	const Circle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	f32 x = shape1.getRealPosition(transform1);
	float3 center = shape2.getRealCenter(transform2);
	f32 radius = shape2.getRealRadius(transform2);
	f32 distance = std::abs(center.x - x);
	if (distance > radius + 0.0001f) {
		return std::nullopt;
	}
	float3 normal = { 1.0f, 0.0f, 0.0f };
	f32 depth = radius - distance;
	return Collision(normal, depth, CollisionState::CurrentlyFound);
}

std::optional<Collision> checkCollision(
	const VerticalLine& shape1,
	const Triangle& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	f32 x = shape1.getRealPosition(transform1);
	std::vector<float3> vertices = shape2.getRealVertices(transform2);
	return checkVerticalLineAndPolygon(x, vertices);
}

std::optional<Collision> checkCollision(
	const VerticalLine& shape1,
	const OBB& shape2,
	const TransformComponent& transform1,
	const TransformComponent& transform2
) {
	f32 x = shape1.getRealPosition(transform1);
	std::vector<float3> vertices = shape2.getRealVertices(transform2);
	return checkVerticalLineAndPolygon(x, vertices);
}

} // namespace arch::physics
