#include <archimedes/physics/collisions/PointAlgorithms.h>
#include <archimedes/physics/Helpers.h>

namespace arch::physics {

f32 getSumOfTriangleAreas(const std::vector<float3>& vertices, float3 point) {
    f32 areasSum = 0.0f;
    for (i32 i = 0; i < vertices.size(); i++) {
        float3 vertex1 = vertices[i];
        float3 vertex2 = vertices[(i + 1) % vertices.size()];
        areasSum += getTriangleArea(vertex1, vertex2, point);
    }
    return areasSum;
}

bool containsPoint(const OBB& shape, float3 point, const TransformComponent& transform) {
    std::vector<float3> vertices = shape.getRealVertices(transform);
    f32 areasSum = getSumOfTriangleAreas(vertices, point);
    f32 obbArea = getTriangleArea(vertices[0], vertices[1], vertices[2]) * 2.0f;
    return std::abs(areasSum - obbArea) < 0.0001f;
}

bool containsPoint(const Triangle& shape, float3 point, const TransformComponent& transform) {
    std::vector<float3> vertices = shape.getRealVertices(transform);
    f32 areasSum = getSumOfTriangleAreas(vertices, point);
    f32 obbArea = getTriangleArea(vertices[0], vertices[1], vertices[2]);
    return std::abs(areasSum - obbArea) < 0.0001f;
}

bool containsPoint(const Circle& shape, float3 point, const TransformComponent& transform) {
    float3 center = shape.getRealCenter(transform);
    return glm::distance2(center, point) < std::pow(shape.radius, 2) + 0.0001f;
}

bool containsPoint(const HorizontalLine& shape, float3 point, const TransformComponent& transform) {
    return false;
}

bool containsPoint(const VerticalLine& shape, float3 point, const TransformComponent& transform) {
    return false;
}

} // namespace arch::physics