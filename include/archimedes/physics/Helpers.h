#pragma once
#include <archimedes/Mmath.h>

namespace arch::physics {

inline float3 getConvexPolygonNorm(float3 edgeVertex1, float3 edgeVertex2, float3 polygonOtherVertex){
    float3 edge = edgeVertex1 - edgeVertex2;
    float3 candidate1 = { -edge.y, edge.x, 0.0f };
    float3 candidate2 = -candidate1;
    if (glm::dot(candidate1, polygonOtherVertex - edgeVertex1) > 0) {
        return candidate2;
    }
    return candidate1;
}

}