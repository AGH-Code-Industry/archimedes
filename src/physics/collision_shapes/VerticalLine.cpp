#include <archimedes/Mmath.h>
#include <archimedes/physics/collision_shapes/VerticalLine.h>

namespace arch::physics {

VerticalLine::VerticalLine(f32 x): x(x) {}

f32 VerticalLine::getRealPosition(const TransformComponent& transform) const {
	float3 axisPoint = { x, 0.0f, 0.0f };
	Mat4x4 model = transform.getTransformMatrix();
	axisPoint = float3(model * float4(axisPoint, 1.0f));
	return axisPoint.x;
}

} // namespace arch::physics
