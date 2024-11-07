#include <audio/Mixer.h>
#include <audio/AudioException.h>

namespace arch::audio {
void Mixer::initialize(){
}

void Mixer::changeSourcePitch(int index, ALfloat pitch) {
	_mutex->lock();
	if(index < 0 || index >= _sources->size()) {
		throw AudioException("Mixer: index out of range");
	}
	if(pitch < 0.0f) {
		throw AudioException("Pitch shouldn't be negative");
	}
	_sources->at(index).pitch = pitch;
	_mutex->unlock();
}

void Mixer::changeSourceGain(int index, ALfloat gain) {
	_mutex->lock();
	if(index < 0 || index >= _sources->size()) {
		throw AudioException("Mixer: index out of range");
	}
	if(gain < 0.0f) {
		throw AudioException("Gain shouldn't be negative");
	}
	_sources->at(index).gain = gain;
	_mutex->unlock();
}

void Mixer::changeSourcePosition(int index, ALfloat positionX, ALfloat positionY) {
	_mutex->lock();
	if(index < 0 || index >= _sources->size()) {
		throw AudioException("Mixer: index out of range");
	}
	_sources->at(index).positionX = positionX;
	_sources->at(index).positionY = positionY;
	_mutex->unlock();
}

void Mixer::changeSourceVelocity(int index, ALfloat velocityX, ALfloat velocityY) {
	_mutex->lock();
	if(index < 0 || index >= _sources->size()) {
		throw AudioException("Mixer: index out of range");
	}
	_sources->at(index).velocityX = velocityX;
	_sources->at(index).velocityY = velocityY;
	_mutex->unlock();
}

void Mixer::changeSourceIsLooped(int index, bool isLooped) {
	_mutex->lock();
	if(index < 0 || index >= _sources->size()) {
		throw AudioException("Mixer: index out of range");
	}
	_sources->at(index).isLooped = isLooped;
	_mutex->unlock();
}

void Mixer::changeListenerMasterGain(ALfloat masterGain) {
	_mutex->lock();
	if(masterGain < 0.0f) {
		throw AudioException("Master gain shouldn't be negative");
	}
	_listener->masterGain = masterGain;
	_mutex->unlock();
}

void Mixer::changeListenerPosition(ALfloat positionX, ALfloat positionY) {
	_mutex->lock();
	_listener->positionX = positionX;
	_listener->positionY = positionY;
	_mutex->unlock();
}

void Mixer::changeListenerVelocity(ALfloat velocityX, ALfloat velocityY) {
	_mutex->lock();
	_listener->velocityX = velocityX;
	_listener->velocityY = velocityY;
	_mutex->unlock();
}

}