#pragma once

#include <archimedes/Gfx.h>

namespace arch {

/// @brief Class of a camera
class Camera {
public:
	/// @brief Camera bounds
	struct Bounds {
		/// @brief Bottom left bound
		float2 bottomLeft;
		/// @brief Top right bound
		float2 topRight;
	};

	/// @brief Constructor
	Camera() noexcept;

	float2 pos() const noexcept;
	void pos(float2 newPosition) noexcept;
	void move(float2 deltaPos) noexcept;

	float2 extents() const noexcept;
	void extents(float2 newExtents) noexcept;
	void changeExtents(float2 delta) noexcept;

	float2 rawExtents() const noexcept;
	void rawExtents(float2 newExtents) noexcept;
	void changeRawExtents(float2 delta) noexcept;

	Bounds bounds() const noexcept;
	void bounds(Bounds newBounds) noexcept;

	Bounds rawBounds() const noexcept;
	void rawBounds(Bounds newBounds) noexcept;

	float2 zoom() const noexcept;
	void zoom(float2 newZoom) noexcept;
	void zoom(float newZoom) noexcept;

	void zoomIn(float2 modifier) noexcept;
	void zoomIn(float modifier) noexcept;
	void zoomOut(float2 modifier) noexcept;
	void zoomOut(float modifier) noexcept;

	float rotation() const noexcept;
	float rotationDeg() const noexcept;
	void rotation(float newRotation) noexcept;
	void rotationDeg(float newRotation) noexcept;
	void rotate(float delta) noexcept;
	void rotateDeg(float delta) noexcept;

	float2 screenToWorldPos(float2 screenPos) const noexcept;
	float2 normalToWorldPos(float2 normalizedDeviceCoordinates) const noexcept;

	const Ref<gfx::Buffer>& buffer() const noexcept;

private:
	void _updateExtents() noexcept;
	void _updateSinCos() noexcept;
	void _updateBuffer() noexcept;

	Ref<gfx::Buffer> _buffer;
	Ref<Window> _window;
	float2 _rawExtents;
	float2 _extents;
	float2 _position;
	float2 _zoom;
	float _rotation;
	float _sinRot;
	float _cosRot;
};

} // namespace arch
