#include <audio/Source.h>
#include <audio/Calls.hpp>

namespace arch::audio {
void Source::_updateSoundAttributes(){
	alCall(alSourcef, _source, AL_PITCH, pitch);
	alCall(alSourcef, _source, AL_GAIN, gain);
	alCall(alSource3f, _source, AL_POSITION, positionX, positionY, 0);
	alCall(alSource3f, _source, AL_VELOCITY, velocityX, velocityY, 0);
	alCall(alSourcei, _source, AL_LOOPING, AL_FALSE);
}

bool Source::_initiallyLoadSound() {
	Clip& clip = _soundBank->getClip(_clipPath);
	ALenum format = clip.getFormat();
	std::size_t bufferElements = clip.getBufferElements();
	ALint sampleRate = clip.getSampleRate();
	bool isEndFound = false;
	for(int i=0; i<4; i++) {
		isEndFound |= clip.fillBuffer(_loadingBuffer, _cursor, isLooped);
		alCall(alBufferData, _buffers[i], format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);
	}
	return isEndFound;
}

bool Source::_loadSound() {
	Clip& clip = _soundBank->getClip(_clipPath);
	ALenum format = clip.getFormat();
	std::size_t bufferElements = clip.getBufferElements();
	ALint sampleRate = clip.getSampleRate();

	ALint buffersProcessed = 0;
	alCall(alGetSourcei, _source, AL_BUFFERS_PROCESSED, &buffersProcessed);

	if(buffersProcessed <= 0) {
		return false;
	}

	bool isEndFound = false;
	for(int i=0; i<buffersProcessed; i++) {
		ALuint buffer;
		alCall(alSourceUnqueueBuffers, _source, 1, &buffer);
		isEndFound |= clip.fillBuffer(_loadingBuffer, _cursor, isLooped);
		alCall(alBufferData, buffer, format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);
		alCall(alSourceQueueBuffers, _source, 1, &buffer);
	}
	return isEndFound;
}

void Source::_prepareLoadingBuffer() {
	Clip& clip = _soundBank->getClip(_clipPath);
	const std::size_t bufferElements = clip.getBufferElements();
	_loadingBuffer.reserve(bufferElements);
	for(int i=0; i<bufferElements; i++) {
		_loadingBuffer.push_back(0);
	}
}

SourceState Source::getState() {
	return _state;
}

void Source::play(){
	if(_state != SourceState::waiting) {
		throw AudioException("Can't play Source: it's not waiting");
	}
	_state = SourceState::playing;
	_updateSoundAttributes();
	_isEndFound = _initiallyLoadSound();
	alCall(alSourceQueueBuffers, _source, 4, &_buffers[0]);
	alCall(alSourcePlay, _source);
}

void Source::update(){
	if(_state != SourceState::playing) {
		throw AudioException("Can't update Source: it's not playing");
	}
	ALenum alState;
	alCall(alGetSourcei, _source, AL_SOURCE_STATE, &alState);
	_updateSoundAttributes();
	if(isLooped || !_isEndFound) {
		_isEndFound = _loadSound();
	}
	if(alState != AL_PLAYING) {
		stop();
	}
}

void Source::stop() {
	if(_state != SourceState::playing) {
		throw AudioException("Can't stop Source: it's not playing");
	}
	_state = SourceState::stopped;
	alCall(alSourceStop, _source);
}

void Source::pausePlaying(){
	if(_state != SourceState::playing) {
		throw AudioException("Can't pause Source: it's not playing");
	}
	_state = SourceState::paused;
	alCall(alSourcePause, _source);
}

void Source::continuePlaying() {
	if(_state != SourceState::paused) {
		throw AudioException("Can't continue Source: it's not paused");
	}
	_state = SourceState::playing;
	alCall(alSourcePlay, _source);
}

void Source::activate(){
	alCall(alGenBuffers, 4, &_buffers[0]);
	alCall(alGenSources, 1, &_source);
	_prepareLoadingBuffer();
}

void Source::deactivate() {
	alCall(alDeleteSources, 1, &_source);
	alCall(alDeleteBuffers, 4, &_buffers[0]);
}
}
