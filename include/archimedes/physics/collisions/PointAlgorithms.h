#pragma once

#include <archimedes/physics/collisions/collision_shapes/OBB.h>
#include <archimedes/physics/collisions/collision_shapes/Triangle.h>
#include <archimedes/physics/collisions/collision_shapes/VerticalLine.h>
#include <archimedes/physics/collisions/collision_shapes/HorizontalLine.h>
#include <archimedes/physics/collisions/collision_shapes/Circle.h>

namespace arch::physics {
bool containsPoint(const OBB& shape, float3 point, const TransformComponent& transform);
bool containsPoint(const Triangle& shape, float3 point, const TransformComponent& transform);
bool containsPoint(const Circle& shape, float3 point, const TransformComponent& transform);
bool containsPoint(const HorizontalLine& shape, float3 point, const TransformComponent& transform);
bool containsPoint(const VerticalLine& shape, float3 point, const TransformComponent& transform);
} // namespace arch::physics