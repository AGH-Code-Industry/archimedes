#include <audio/Calls.hpp>

#include <audio/Listener.h>

namespace arch::audio {

void Listener::update(const ListenerComponent& component) {
	alCall(alListenerf, AL_GAIN, component.masterGain);
	alCall(alListener3f, AL_POSITION, component.positionX, component.positionY, 0);
	alCall(alListener3f, AL_VELOCITY, component.velocityX, component.velocityY, 0);
}

} // namespace arch::audio
