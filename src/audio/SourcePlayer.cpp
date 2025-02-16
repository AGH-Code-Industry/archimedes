#include <audio/Calls.hpp>

#include <audio/SourcePlayer.h>

namespace arch::audio {

void SourcePlayer::update(const AudioSourceComponent& source) {
	_pitch = source.pitch;
	_gain = source.gain;
	_positionX = source.positionX;
	_positionY = source.positionY;
	_velocityX = source.velocityX;
	_velocityY = source.velocityY;
	_isLooped = source.isLooped;
	// if (source.path != _clipPath) {
	// 	if (_clipPath == "") {
	// 		_clipPath = source.path;
	// 	} else {
	// 		//TODO: flagged (should be a better way to test this)
	// 		//I don't want to allow for changing clip path DURING play time,
	// 		//but it should be allowed if the SourcePlayer is not in use
	// 		//maybe a different method for updating the clip path, only during startup?
	// 		throw AudioException("Clip path should be changed only once per source");
	// 	}
	// }
}

void SourcePlayer::setClipPath(const std::string& clipPath) {
	_clipPath = clipPath;
}


void SourcePlayer::_updateSoundAttributes() {
	alCall(alSourcef, _source, AL_PITCH, _pitch);
	alCall(alSourcef, _source, AL_GAIN, _gain);
	alCall(alSource3f, _source, AL_POSITION, _positionX, _positionY, 0);
	alCall(alSource3f, _source, AL_VELOCITY, _velocityX, _velocityY, 0);
	alCall(alSourcei, _source, AL_LOOPING, AL_FALSE);
}

bool SourcePlayer::_initiallyLoadSound() {
	Clip& clip = _soundBank->getClip(_clipPath);
	ALenum format{};
	switch(clip.getFormat()) {
		case mono:
			format = AL_FORMAT_MONO16;
			break;
		case stereo:
			format = AL_FORMAT_STEREO16;
			break;
	}
	std::size_t bufferElements = clip.getBufferElements();
	ALint sampleRate = clip.getSampleRate();
	bool isEndFound = false;
	for (int i = 0; i < 4; i++) {
		isEndFound |= clip.fillBuffer(_loadingBuffer, _cursor, _isLooped);
		alCall(alBufferData, _buffers[i], format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);
	}
	return isEndFound;
}

bool SourcePlayer::_loadSound() {
	Clip& clip = _soundBank->getClip(_clipPath);
	ALenum format{};
	switch(clip.getFormat()) {
		case mono:
			format = AL_FORMAT_MONO16;
			break;
		case stereo:
			format = AL_FORMAT_STEREO16;
			break;
	}
	std::size_t bufferElements = clip.getBufferElements();
	ALint sampleRate = clip.getSampleRate();
	ALint buffersProcessed;
	alCall(alGetSourcei, _source, AL_BUFFERS_PROCESSED, &buffersProcessed);
	bool isEndFound = false;
	for (int i = 0; i < buffersProcessed; i++) {
		ALuint buffer;
		alCall(alSourceUnqueueBuffers, _source, 1, &buffer);
		isEndFound |= clip.fillBuffer(_loadingBuffer, _cursor, _isLooped);
		alCall(alBufferData, buffer, format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);
		alCall(alSourceQueueBuffers, _source, 1, &buffer);
	}
	return isEndFound;
}

void SourcePlayer::_prepareLoadingBuffer() {
	Clip& clip = _soundBank->getClip(_clipPath);
	const std::size_t bufferElements = clip.getBufferElements();
	_loadingBuffer.resize(bufferElements, 0);
	_cursor = 0;
}

bool SourcePlayer::run() {
	ALenum alState;
	alCall(alGetSourcei, _source, AL_SOURCE_STATE, &alState);
	_updateSoundAttributes();
	bool isStopped = false;
	switch (alState) {
		case AL_PLAYING: _doNextFrame(); break;
		case AL_PAUSED:	 _continuePlaying(); break;
		case AL_STOPPED:
			// source.stop();
			isStopped = true;
			break;
		case AL_INITIAL: _startFromBeginning(); break;
		default:		 throw AudioException("Invalid state");
	}
	return isStopped;
}

void SourcePlayer::_startFromBeginning() {
	_prepareLoadingBuffer();
	_isEndFound = _initiallyLoadSound();
	alCall(alSourceQueueBuffers, _source, 4, &_buffers[0]);
	alCall(alSourcePlay, _source);
}

void SourcePlayer::_doNextFrame() {
	if (_isLooped || !_isEndFound) {
		_isEndFound = _loadSound();
	}
}

bool SourcePlayer::stopPlaying() {
	_gain = 0.0f;
	ALenum alState;
	alCall(alGetSourcei, _source, AL_SOURCE_STATE, &alState);
	if (alState == AL_PAUSED) {
		_continuePlaying();
	}
	alCall(alSourcef, _source, AL_GAIN, _gain);
	ALint buffersProcessed, buffersQueued;
	alCall(alGetSourcei, _source, AL_BUFFERS_PROCESSED, &buffersProcessed);
	alCall(alGetSourcei, _source, AL_BUFFERS_QUEUED, &buffersQueued);
	if (buffersProcessed < buffersQueued) {
		return false;
	}
	alCall(alSourceUnqueueBuffers, _source, 4, &_buffers[0]);
	alCall(alSourceRewind, _source);
	_clipPath = "";
	return true;
}

void SourcePlayer::pausePlaying() {
	alCall(alSourcePause, _source);
}

void SourcePlayer::_continuePlaying() {
	alCall(alSourcePlay, _source);
}

void SourcePlayer::initialize(SoundBank* soundBank) {
	_soundBank = soundBank;
	alCall(alGenBuffers, 4, &_buffers[0]);
	alCall(alGenSources, 1, &_source);
}

SourcePlayer::~SourcePlayer() {
	_soundBank = nullptr;
	alCall(alDeleteSources, 1, &_source);
	alCall(alDeleteBuffers, 4, &_buffers[0]);
}
} // namespace arch::audio
