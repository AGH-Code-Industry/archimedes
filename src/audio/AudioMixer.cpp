#include <audio/AudioMixer.h>
#include <audio/AudioException.h>

namespace arch::audio {
void AudioMixer::initialize(std::vector<AudioSource>* audioSources, AudioListener* listener, std::mutex* mutex){
	_audioSources = audioSources;
	_listener = listener;
	_mutex = mutex;
}

void AudioMixer::changeSourcePitch(int index, ALfloat pitch) {
	_mutex->lock();
	if(index < 0 || index >= _audioSources->size()) {
		throw AudioException("AudioMixer: index out of range");
	}
	if(pitch < 0.0f) {
		throw AudioException("Pitch shouldn't be negative");
	}
	_audioSources->at(index).pitch = pitch;
	_mutex->unlock();
}

void AudioMixer::changeSourceGain(int index, ALfloat gain) {
	_mutex->lock();
	if(index < 0 || index >= _audioSources->size()) {
		throw AudioException("AudioMixer: index out of range");
	}
	if(gain < 0.0f) {
		throw AudioException("Gain shouldn't be negative");
	}
	_audioSources->at(index).gain = gain;
	_mutex->unlock();
}

void AudioMixer::changeSourcePosition(int index, ALfloat positionX, ALfloat positionY) {
	_mutex->lock();
	if(index < 0 || index >= _audioSources->size()) {
		throw AudioException("AudioMixer: index out of range");
	}
	_audioSources->at(index).positionX = positionX;
	_audioSources->at(index).positionY = positionY;
	_mutex->unlock();
}

void AudioMixer::changeSourceVelocity(int index, ALfloat velocityX, ALfloat velocityY) {
	_mutex->lock();
	if(index < 0 || index >= _audioSources->size()) {
		throw AudioException("AudioMixer: index out of range");
	}
	_audioSources->at(index).velocityX = velocityX;
	_audioSources->at(index).velocityY = velocityY;
	_mutex->unlock();
}

void AudioMixer::changeSourceIsLooped(int index, bool isLooped) {
	_mutex->lock();
	if(index < 0 || index >= _audioSources->size()) {
		throw AudioException("AudioMixer: index out of range");
	}
	_audioSources->at(index).isLooped = isLooped;
	_mutex->unlock();
}

void AudioMixer::changeListenerMasterGain(ALfloat masterGain) {
	_mutex->lock();
	if(masterGain < 0.0f) {
		throw AudioException("Master gain shouldn't be negative");
	}
	_listener->masterGain = masterGain;
	_mutex->unlock();
}

void AudioMixer::changeListenerPosition(ALfloat positionX, ALfloat positionY) {
	_mutex->lock();
	_listener->positionX = positionX;
	_listener->positionY = positionY;
	_mutex->unlock();
}

void AudioMixer::changeListenerVelocity(ALfloat velocityX, ALfloat velocityY) {
	_mutex->lock();
	_listener->velocityX = velocityX;
	_listener->velocityY = velocityY;
	_mutex->unlock();
}

}