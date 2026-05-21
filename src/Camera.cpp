#include <archimedes/Camera.h>

namespace arch {

Camera::Camera() noexcept {
	auto&& renderer = *gfx::Renderer::current();
	_window = renderer.getWindow();

	_buffer = renderer.getBufferManager()->createBuffer(gfx::BufferType::uniform);
	_rawExtents = _window->size() / 2;
	_position = {};
	_zoom = { 1, 1 };
	_rotation = 0;
	_sinRot = 0;
	_cosRot = 1;

	_updateExtents();
}

void Camera::_updateBuffer() noexcept {
	auto viewProj = glm::ortho(-_extents.x, _extents.x, -_extents.y, _extents.y) *
		glm::rotate(glm::mat4(1.f), _rotation, float3(0, 0, 1)) *
		glm::translate(glm::mat4(1.f), float3(-_position.x, -_position.y, 0.f));

	_buffer->setData(&viewProj, sizeof(viewProj));
}

void Camera::_updateExtents() noexcept {
	_extents = _rawExtents * _zoom;
	_updateBuffer();
}

void Camera::_updateSinCos() noexcept {
	_sinRot = std::sin(_rotation);
	_cosRot = std::cos(_rotation);
	_updateBuffer();
}

float2 Camera::pos() const noexcept {
	return _position;
}

void Camera::setPos(float2 newPosition) noexcept {
	_position = newPosition;
	_updateBuffer();
}

void Camera::move(float2 delta) noexcept {
	setPos(pos() + delta);
}

float2 Camera::extents() const noexcept {
	return _extents;
}

void Camera::setExtents(float2 newExtents) noexcept {
	_rawExtents = newExtents;
	_zoom = { 1, 1 };
	_updateExtents();
}

void Camera::changeExtents(float2 delta) noexcept {
	setExtents(extents() + delta);
}

float2 Camera::rawExtents() const noexcept {
	return _rawExtents;
}

	_rawExtents = newExtents;
	_updateExtents();
}

void Camera::changeRawExtents(float2 delta) noexcept {
	setRawExtents(rawExtents() + delta);
}

Camera::Bounds Camera::bounds() const noexcept {
	return Bounds{ .bottomLeft = _position - _extents, .topRight = _position + _extents };
}

	_position = (newBounds.bottomLeft + newBounds.topRight) / 2.f;
	_rawExtents = (newBounds.topRight - newBounds.bottomLeft) / 2.f;
	_zoom = { 1, 1 };
	_updateExtents();
}

Camera::Bounds Camera::rawBounds() const noexcept {
	return Bounds{ .bottomLeft = _position - _rawExtents, .topRight = _position + _rawExtents };
}

void Camera::setRawBounds(Bounds newBounds) noexcept {
	_position = (newBounds.bottomLeft + newBounds.topRight) / 2.f;
	_rawExtents = (newBounds.topRight - newBounds.bottomLeft) / 2.f;
	_updateExtents();
}

float2 Camera::zoom() const noexcept {
	return _zoom;
}

void Camera::setZoom(float newZoom) noexcept {
	_zoom = newZoom;
	_updateExtents();
}

void Camera::zoom(float newZoom) noexcept {
	zoom({ newZoom, newZoom });
}

void Camera::zoomIn(float2 modifier) noexcept {
	zoom(zoom() / modifier);
}

void Camera::zoomIn(float modifier) noexcept {
	zoom(zoom() / modifier);
}

void Camera::zoomOut(float2 modifier) noexcept {
	zoom(zoom() * modifier);
}

void Camera::zoomOut(float modifier) noexcept {
	setZoom(zoom() * modifier);
}

float Camera::rotation() const noexcept {
	return _rotation;
}

float Camera::rotationDeg() const noexcept {
	return glm::degrees(_rotation);
}

void Camera::rotation(float newRotation) noexcept {
	_rotation = newRotation;
	_updateSinCos();
}

void Camera::rotationDeg(float newRotation) noexcept {
	_rotation = glm::radians(newRotation);
	_updateSinCos();
}

void Camera::rotate(float delta) noexcept {
}

void Camera::rotateDeg(float delta) noexcept {
	rotation(rotation() + glm::radians(delta));
	setRotation(rotation() + delta);
}

float2 Camera::screenToWorldPos(float2 screenPos) const noexcept {
	return normalToWorldPos(screenPos / (float2)_window->size() * 2.f - 1.f);
}

float2 Camera::normalToWorldPos(float2 normalizedDeviceCoordinates) const noexcept {
	normalizedDeviceCoordinates *= _extents;
	return float2{ _cosRot * normalizedDeviceCoordinates.x + _sinRot * normalizedDeviceCoordinates.y,
				   -_sinRot * normalizedDeviceCoordinates.x + _cosRot * normalizedDeviceCoordinates.y } +
		_position;
}

const Ref<gfx::Buffer>& Camera::buffer() const noexcept {
	return _buffer;
}

} // namespace arch
