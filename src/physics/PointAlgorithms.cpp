#include <archimedes/physics/collisions/PointAlgorithms.h>

namespace arch::physics {
bool containsPoint(const OBB& shape, float3 point, const TransformComponent& transform) {
    std::vector<float3> axes = shape.getSeparatingAxes(transform);
    for (auto& axis : axes) {
        float2 projection = shape.getProjection(axis, transform);
        f32 pointProjection = glm::dot(point - transform.position, axis);
        if (pointProjection < projection.x || pointProjection > projection.y) {
            return false;
        }
    }
    return true;
}

bool containsPoint(const Triangle& shape, float3 point, const TransformComponent& transform) {
    std::vector<float3> axes = shape.getSeparatingAxes(transform);
    for (auto& axis : axes) {
        float2 projection = shape.getProjection(axis, transform);
        f32 pointProjection = glm::dot(point - transform.position, axis);
        if (pointProjection < projection.x || pointProjection > projection.y) {
            return false;
        }
    }
    return true;
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