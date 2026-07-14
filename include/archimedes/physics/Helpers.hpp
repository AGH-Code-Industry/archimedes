#pragma once
#include <archimedes/Mmath.h>

namespace arch::physics {

/// @brief Used by the collision system
constexpr f32 COLLISIONS_EPSILON = 0.0001f;

/// @brief Get the normal vector for a given edge
inline float2 getConvexPolygonNorm(float2 edgeVertex1, float2 edgeVertex2, float2 polygonOtherVertex) {
	float2 edge = edgeVertex1 - edgeVertex2;
	float2 candidate1 = { -edge.y, edge.x };
	float2 candidate2 = -candidate1;
	if (glm::dot(candidate1, polygonOtherVertex - edgeVertex1) > 0) {
		return candidate2;
	}
	return candidate1;
}

/// @brief Get the area of a triangle, using cross product
inline f32 getTriangleArea(float2 vertex1, float2 vertex2, float2 vertex3) {
	float2 edge1 = vertex2 - vertex1;
	float2 edge2 = vertex3 - vertex1;
	float3 crossProduct = glm::cross(float3(edge1, 0.0f), float3(edge2, 0.0f));
	return 0.5f * glm::length(crossProduct);
}

/// @brief Check if two vectors point in the same direction
/// Sometimes you want to negate a vector, e.g. if it has to align to your speed
inline bool areVectorsSameDirection(float2 vector1, float2 vector2) {
	return glm::dot(vector1, vector2) > 0;
}

///@brief Divide a convex polygon into triangles using the point (all of them use it as a vertex)
template<std::size_t ArraySize>
inline f32 getSumOfTriangleAreas(std::array<float2, ArraySize> polygonVertices, float2 point) {
	f32 sizesSum = 0.0f;
	for(i32 i=0; i<ArraySize; i++){
		float2 vertex1 = polygonVertices[i];
		float2 vertex2 = polygonVertices[(i + 1) % ArraySize];
		sizesSum += getTriangleArea(vertex1, vertex2, point);
	}
	return sizesSum;
}

} // namespace arch::physics
