#pragma once

#include <archimedes/Gfx.h>

namespace arch {

/// @brief Camera
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
	/// @details Default state is centered at (0, 0) with extents half the window size
	Camera() noexcept;

	/// @brief Returns position
	float2 pos() const noexcept;
	/// @brief Sets new position
	void setPos(float2 newPosition) noexcept;
	/// @brief Moves camera
	void move(float2 deltaPos) noexcept;

	/// @brief Returns real extents
	/// @details Extents are half the size of camera's viewport, accounting for zoom
	float2 extents() const noexcept;
	/// @brief Sets real extents
	/// @details Resets zoom
	void setExtents(float2 newExtents) noexcept;
	/// @brief Expands real extents
	/// @details Resets zoom
	void changeExtents(float2 deltaExtents) noexcept;

	/// @brief Returns raw extents
	/// @details Raw meaning excluding the zoom
	float2 rawExtents() const noexcept;
	/// @brief Sets raw extents
	/// @details Does not change the zoom
	void setRawExtents(float2 newExtents) noexcept;
	/// @brief Expands raw extents
	/// @details Does not change the zoom
	void changeRawExtents(float2 deltaRawExtents) noexcept;

	/// @brief Returns real bounds
	/// @details Bounds are the camera's axis-aligned viewport, accounting for zoom
	Bounds bounds() const noexcept;
	/// @brief Sets real bounds
	/// @details Resets zoom
	void setBounds(Bounds newBounds) noexcept;

	/// @brief Returns raw bounds
	/// @details Raw meaning excluding the zoom
	Bounds rawBounds() const noexcept;
	/// @brief Sets raw bounds
	/// @details Does not change the zoom
	void setRawBounds(Bounds newBounds) noexcept;

	/// @brief Returns zoom
	float zoom() const noexcept;
	/// @brief Sets zoom
	void setZoom(float newZoom) noexcept;
	/// @brief Zooms in
	void zoomIn(float modifier) noexcept;
	/// @brief Zooms out
	void zoomOut(float modifier) noexcept;

	/// @brief Returns rotation in radians
	float rotation() const noexcept;
	/// @brief Sets rotation in radians
	void setRotation(float newRotation) noexcept;
	/// @brief Rotates camera by given amount of radians
	void rotate(float delta) noexcept;

	/// @brief Computes world position of a given window position
	float2 screenToWorldPos(float2 screenPos) const noexcept;
	/// @brief Computes world position of a given window position (Normalized Device Coordinates)
	float2 normalToWorldPos(float2 normalizedDeviceCoordinates) const noexcept;

	/// @brief Returns uniform buffer with view-projection matrix
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
	float _zoom;
	float _rotation;
	float _sinRot;
	float _cosRot;
};

} // namespace arch
