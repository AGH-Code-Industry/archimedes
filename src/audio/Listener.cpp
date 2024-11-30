#include <audio/Listener.h>
#include <audio/Calls.hpp>

namespace arch::audio {

Listener::Listener(ALfloat masterGain, ALfloat positionX, ALfloat positionY, ALfloat velocityX,
							ALfloat velocityY): masterGain(masterGain), positionX(positionX), positionY(positionY),
							velocityX(velocityX), velocityY(velocityY){
	update();
}

void Listener::update() {
	alCall(alListenerf, AL_GAIN, masterGain);
	alCall(alListener3f, AL_POSITION, positionX, positionY, 0);
	alCall(alListener3f, AL_VELOCITY, velocityX, velocityY, 0);
}

}