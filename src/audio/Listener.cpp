#include <audio/Calls.hpp>

#include <audio/Listener.h>

namespace arch::audio {

Listener::Listener(float masterGain, float positionX, float positionY, float velocityX, float velocityY):
	masterGain(masterGain),
	positionX(positionX),
	positionY(positionY),
	velocityX(velocityX),
	velocityY(velocityY) {
	update();
}

void Listener::update() {
	alCall(alListenerf, AL_GAIN, masterGain);
	alCall(alListener3f, AL_POSITION, positionX, positionY, 0);
	alCall(alListener3f, AL_VELOCITY, velocityX, velocityY, 0);
}

} // namespace arch::audio
