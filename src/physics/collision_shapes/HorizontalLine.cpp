#include <archimedes/physics/collision_shapes/HorizontalLine.h>
#include <archimedes/Mmath.h>


namespace arch::physics {

	HorizontalLine::HorizontalLine(f32 y) : y(y) {}

	f32 HorizontalLine::getRealPosition(const TransformComponent& transform) const {
        float3 axisPoint = { 0.0f, y, 0.0f };
        Mat4x4 model = transform.getTransformMatrix();
        axisPoint = float3(model * float4(axisPoint, 1.0f));
		return axisPoint.y;
	}

} // namespace arch::physics
