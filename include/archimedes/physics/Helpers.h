#pragma once
#include <archimedes/Mmath.h>

namespace arch::physics {

/// @brief Get the normal vector for a given edge
inline float3 getConvexPolygonNorm(float3 edgeVertex1, float3 edgeVertex2, float3 polygonOtherVertex) {
	float3 edge = edgeVertex1 - edgeVertex2;
	float3 candidate1 = { -edge.y, edge.x, 0.0f };
	float3 candidate2 = -candidate1;
	if (glm::dot(candidate1, polygonOtherVertex - edgeVertex1) > 0) {
		return candidate2;
	}
	return candidate1;
}


/// @brief Get the area of a triangle, using the shoelace formula
inline f32 getTriangleArea(float3 vertex1, float3 vertex2, float3 vertex3) {
	return 0.5f * std::abs(vertex1.x * (vertex2.y - vertex3.y) +
		vertex2.x * (vertex3.y - vertex1.y) +
		vertex3.x * (vertex1.y - vertex2.y));
}

/// @brief Check if two vectors point in the same direction 
/// Sometimes you want to negate a vector, e.g. if it has to align to your speed
inline bool areVectorsSameDirection(float3 vector1, float3 vector2) {
	return glm::dot(vector1, vector2) > 0;
}

} // namespace arch::physics
