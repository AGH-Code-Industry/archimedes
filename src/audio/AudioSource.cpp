#include <audio/AudioSource.h>
#include <audio/Calls.hpp>

namespace arch::audio{

	void AudioSource::_updateSoundAttributes(){
		alCall(alSourcef, _source, AL_PITCH, _pitch);
		alCall(alSourcef, _source, AL_GAIN, _gain);
		alCall(alSource3f, _source, AL_POSITION, _positionX, _positionY, 0);
		alCall(alSource3f, _source, AL_VELOCITY, _velocityX, _velocityY, 0);
		alCall(alSourcei, _source, AL_LOOPING, AL_FALSE);
	}

	bool AudioSource::_initiallyLoadSound() {
		Clip& clip = _soundBank->getClip(_clipPath);
		ALenum format = clip.getFormat();
		std::size_t bufferElements = clip.getBufferElements();
		ALint sampleRate = clip.getSampleRate();
		bool isEndFound = false;
		for(int i=0; i<4; i++) {
			isEndFound |= clip.fillBuffer(_loadingBuffer, _cursor, _isLooped);
			alCall(alBufferData, _buffers[i], format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);
		}
		return isEndFound;
	}

	bool AudioSource::_loadSound() {
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
			isEndFound |= clip.fillBuffer(_loadingBuffer, _cursor, _isLooped);
			alCall(alBufferData, buffer, format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);
			alCall(alSourceQueueBuffers, _source, 1, &buffer);
		}
		return isEndFound;
	}

	void AudioSource::_prepareLoadingBuffer() {
		Clip& clip = _soundBank->getClip(_clipPath);
		const std::size_t bufferElements = clip.getBufferElements();
		_loadingBuffer.reserve(bufferElements);
		for(int i=0; i<bufferElements; i++) {
			_loadingBuffer.push_back(0);
		}
	}

	SourceState AudioSource::getState() {
		return _state;
	}

	void AudioSource::play(){
		if(_state != SourceState::waiting) {
			throw AudioException("Can't play AudioSource: it's not waiting");
		}
		_state = SourceState::playing;
		_updateSoundAttributes();
		_isEndFound = _initiallyLoadSound();
		alCall(alSourceQueueBuffers, _source, 4, &_buffers[0]);
		alCall(alSourcePlay, _source);
	}

	void AudioSource::update(){
		if(_state != SourceState::playing) {
			throw AudioException("Can't update AudioSource: it's not playing");
		}
		ALenum alState;
		alCall(alGetSourcei, _source, AL_SOURCE_STATE, &alState);
		_updateSoundAttributes();
		if(_isLooped || !_isEndFound) {
			_isEndFound = _loadSound();
		}
		if(alState != AL_PLAYING) {
			stop();
		}
	}

	void AudioSource::stop() {
		if(_state != SourceState::playing) {
			throw AudioException("Can't stop AudioSource: it's not playing");
		}
		_state = SourceState::stopped;
		alCall(alSourceStop, _source);
	}

	void AudioSource::pausePlaying(){
		if(_state != SourceState::playing) {
			throw AudioException("Can't pause AudioSource: it's not playing");
		}
		_state = SourceState::paused;
		alCall(alSourcePause, _source);
	}

	void AudioSource::continuePlaying() {
		if(_state != SourceState::paused) {
			throw AudioException("Can't continue AudioSource: it's not paused");
		}
		_state = SourceState::playing;
		alCall(alSourcePlay, _source);
	}

	void AudioSource::activate(){
		alCall(alGenBuffers, 4, &_buffers[0]);
		alCall(alGenSources, 1, &_source);
		_prepareLoadingBuffer();
	}

	void AudioSource::deactivate() {
		alCall(alDeleteSources, 1, &_source);
		alCall(alDeleteBuffers, 4, &_buffers[0]);
	}

void AudioSource::changePitch(ALfloat pitch, std::mutex* mutex) {
		mutex->lock();
		if(pitch < 0.0f) {
			throw AudioException("Pitch shouldn't be negative");
		}
		_pitch = pitch;
		mutex->unlock();
	}

void AudioSource::changeGain(ALfloat gain, std::mutex* mutex) {
		mutex->lock();
		if(gain < 0.0f) {
			throw AudioException("Gain shouldn't be negative");
		}
		_gain = gain;
		mutex->unlock();
	}

void AudioSource::changePosition(ALfloat positionX, ALfloat positionY, std::mutex* mutex) {
		mutex->lock();
		_positionX = positionX;
		_positionY = positionY;
		mutex->unlock();
	}

void AudioSource::changeVelocity(ALfloat velocityX, ALfloat velocityY, std::mutex* mutex) {
		mutex->lock();
		_velocityX = velocityX;
		_velocityY = velocityY;
		mutex->unlock();
	}

void AudioSource::changeIsLooped(bool isLooped, std::mutex* mutex) {
		mutex->lock();
		_isLooped = isLooped;
		mutex->unlock();
	}
}

