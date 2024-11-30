#include <audio/Calls.hpp>

#include <audio/Source.h>

namespace arch::audio {

void Source::update(const SourceComponent& component) {
	_pitch = component.pitch;
	_gain = component.gain;
	_positionX = component.positionX;
	_positionY = component.positionY;
	_velocityX = component.velocityX;
	_velocityY = component.velocityY;
	_isLooped = component.isLooped;
	if (component.path != _clipPath) {
		if (_clipPath == "") {
			_clipPath = component.path;
		} else {
			throw AudioException("Clip path should be changed only once per source");
		}
	}
}

void Source::_updateSoundAttributes() {
	alCall(alSourcef, _source, AL_PITCH, _pitch);
	alCall(alSourcef, _source, AL_GAIN, _gain);
	alCall(alSource3f, _source, AL_POSITION, _positionX, _positionY, 0);
	alCall(alSource3f, _source, AL_VELOCITY, _velocityX, _velocityY, 0);
	alCall(alSourcei, _source, AL_LOOPING, AL_FALSE);
}

bool Source::_initiallyLoadSound() {
	// Logger::debug("before clip");
	Clip& clip = _soundBank->getClip(_clipPath);
	// Logger::debug("before format");
	ALenum format = clip.getFormat();
	// Logger::debug("before buffer elems");
	std::size_t bufferElements = clip.getBufferElements();
	// Logger::debug("before sample rate");
	// Logger::debug("{}", _clipPath);
	ALint sampleRate = clip.getSampleRate();
	bool isEndFound = false;

	ALint queuedBuffers = 0;
	alCall(alGetSourcei, _source, AL_BUFFERS_QUEUED, &queuedBuffers);

	if (queuedBuffers > 0) {
		// Unqueue all buffers before modifying them
		ALuint tempBuffers[4];
		alCall(alSourceUnqueueBuffers, _source, queuedBuffers, tempBuffers);
	}

	for (int i = 0; i < 4; i++) {
		// Logger::debug("before fill");
		isEndFound |= clip.fillBuffer(_loadingBuffer, _cursor, _isLooped);
		// Logger::debug("before bufferdata");
		alCall(alBufferData, _buffers[i], format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);
		// Logger::debug("after iteration {}", i);
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

	if (buffersProcessed <= 0) {
		return false;
	}

	bool localEndFound = false;
	bool isEndFound = false;

	for (int i = 0; i < buffersProcessed; i++) {
		ALuint buffer;
		alCall(alSourceUnqueueBuffers, _source, 1, &buffer);

		if (!isEndFound) {
			// Fill the buffer with new data
			localEndFound = clip.fillBuffer(_loadingBuffer, _cursor, _isLooped);

			alCall(alBufferData, buffer, format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);

			// Re-queue the buffer if there's still data
			alCall(alSourceQueueBuffers, _source, 1, &buffer);
		}
	}

	// Update the global EOF flag
	isEndFound |= localEndFound;

	// If EOF reached and not looping, ensure playback stops
	if (isEndFound && !_isLooped) {
		ALint queuedBuffers = 0;
		alCall(alGetSourcei, _source, AL_BUFFERS_QUEUED, &queuedBuffers);

		// Stop playback if no buffers are left to play
		if (queuedBuffers == 0) {
			alCall(alSourceStop, _source);
		}
	}

	return isEndFound;
}

void Source::_prepareLoadingBuffer() {
	Clip& clip = _soundBank->getClip(_clipPath);
	const std::size_t bufferElements = clip.getBufferElements();
	// Logger::debug("bufferElements: {}", bufferElements);
	_loadingBuffer.resize(bufferElements, 0);
	_cursor = 0;
}

void Source::play(SourceComponent& component) {
	ALenum alState;
	alCall(alGetSourcei, _source, AL_SOURCE_STATE, &alState);

	_updateSoundAttributes();
	if (alState == AL_PLAYING) {
		_doNextFrame(component);
	} else if (alState == AL_PAUSED) {
		_continuePlaying();
	} /*else if (alState == AL_STOPPED) {
		component.stop();
	}*/
	else {
		_startFromBeginning();
	}
}

void Source::_startFromBeginning() {
	_prepareLoadingBuffer();

	ALint queuedBuffers = 0;
	alCall(alGetSourcei, _source, AL_BUFFERS_QUEUED, &queuedBuffers);

	if (queuedBuffers > 0) {
		ALuint tempBuffers[4];
		alCall(alSourceUnqueueBuffers, _source, queuedBuffers, tempBuffers);
	}

	_isEndFound = _initiallyLoadSound();
	alCall(alSourceQueueBuffers, _source, 4, &_buffers[0]);
	alCall(alSourcePlay, _source);
}

void Source::_doNextFrame(SourceComponent& component) {
	ALenum alState;
	alCall(alGetSourcei, _source, AL_SOURCE_STATE, &alState);

	if (alState != AL_PLAYING) {
		// Logger::debug("stop");
		component.stop();
	} else if (_isLooped || !_isEndFound) {
		_isEndFound = _loadSound();
	}
}

void Source::stopPlaying(bool reset) {
	alCall(alSourceStop, _source);
	if (reset) {
		_clipPath = "";
	}
}

void Source::pausePlaying() {
	alCall(alSourcePause, _source);
}

void Source::_continuePlaying() {
	alCall(alSourcePlay, _source);
}

void Source::initialize(SoundBank* soundBank) {
	_soundBank = soundBank;
	alCall(alGenBuffers, 4, &_buffers[0]);
	alCall(alGenSources, 1, &_source);
}

Source::~Source() {
	_soundBank = nullptr;
	alCall(alDeleteSources, 1, &_source);
	alCall(alDeleteBuffers, 4, &_buffers[0]);
}
} // namespace arch::audio
