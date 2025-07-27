#include <audio/Calls.hpp>

#include <audio/Listener.h>

namespace arch::audio {

void Listener::update(const ListenerComponent& component) {
	alCall(alListenerf, AL_GAIN, component.gain);
	alCall(alListener3f, AL_POSITION, component.position.x, component.position.y, 0);
	alCall(alListener3f, AL_VELOCITY, component.velocity.x, component.velocity.y, 0);
}

} // namespace arch::audio
