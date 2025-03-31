#include <audio/Calls.hpp>
#include <audio/Listener.h>

namespace arch::audio {

void Listener::update(const ListenerComponent& component) {
	alCall(alListenerf, AL_GAIN, component.gain);
	alCall(alListener3f, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alCall(alListener3f, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	float orientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
	alCall(alListenerfv, AL_ORIENTATION, orientation);
}

void Listener::update(const ListenerComponent& component, const scene::components::TransformComponent& transform,
	const physics::Moveable& moveable) {
	alCall(alListenerf, AL_GAIN, component.gain);
	alCall(alListener3f, AL_POSITION, transform.position.x, transform.position.y, 0.0f);
	alCall(alListener3f, AL_VELOCITY, moveable.velocity.x, moveable.velocity.y, 0.0f);
	float3 up = transform.rotation * float3(0.0f, 1.0f, 0.0f);
	float orientation[] = {0.0f, 0.0f, -1.0f, up.x, up.y, 0.0f};
	alCall(alListenerfv, AL_ORIENTATION, orientation);
}

} // namespace arch::audio
