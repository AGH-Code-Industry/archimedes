#include <audio/AudioListener.h>
#include <audio/Calls.hpp>

namespace arch::audio {

AudioListener::AudioListener(ALfloat masterGain, ALfloat positionX, ALfloat positionY, ALfloat velocityX,
							ALfloat velocityY): _masterGain(masterGain), _positionX(positionX), _positionY(positionY),
							_velocityX(velocityX), _velocityY(velocityY){
	update();
}

void AudioListener::update() {
	alCall(alListenerf, AL_GAIN, _masterGain);
	alCall(alListener3f, AL_POSITION, _positionX, _positionY, 0);
	alCall(alListener3f, AL_VELOCITY, _velocityX, _velocityY, 0);
}

void AudioListener::_changeMasterGain(ALfloat masterGain, std::mutex* mutex) {
	mutex->lock();
	if(masterGain < 0.0f) {
		throw AudioException("Master gain shouldn't be negative");
	}
	_masterGain = masterGain;
	mutex->unlock();
}

void AudioListener::_changePosition(ALfloat positionX, ALfloat positionY, std::mutex* mutex) {
	mutex->lock();
	_positionX = positionX;
	_positionY = positionY;
	mutex->unlock();
}

void AudioListener::_changeVelocity(ALfloat velocityX, ALfloat velocityY, std::mutex* mutex) {
	mutex->lock();
	_velocityX = velocityX;
	_velocityY = velocityY;
	mutex->unlock();
}
}