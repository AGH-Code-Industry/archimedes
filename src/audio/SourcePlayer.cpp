#include <audio/Calls.hpp>

#include <audio/SourcePlayer.h>
#include <physics/components/Velocity.h>

namespace arch::audio {

void SourcePlayer::setClipPath(const std::string& clipPath) {
	_clipPath = clipPath;
}

void SourcePlayer::clean() {
	_clipPath = "";
	alCall(alSourceRewind, _source);
	alCall(alSourcef, _source, AL_PITCH, 1.0f);
	alCall(alSourcef, _source, AL_GAIN, 1.0f);
	alCall(alSource3f, _source, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alCall(alSource3f, _source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alCall(alSource3f, _source, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
	_isLooped = false;
	alCall(alSourcef, _source, AL_MAX_DISTANCE, std::numeric_limits<float>::max());
	alCall(alSourcef, _source, AL_REFERENCE_DISTANCE, 1.0f);
	alCall(alSourcef, _source, AL_ROLLOFF_FACTOR, 1.0f);
	alCall(alSourcef, _source, AL_CONE_INNER_ANGLE, 360.0f);
	alCall(alSourcef, _source, AL_CONE_OUTER_ANGLE, 360.0f);
	alCall(alSourcef, _source, AL_CONE_OUTER_GAIN, 0.0f);
	_isEndFound = false;
	_cursor = 0;
}

void SourcePlayer::update(const AudioSourceComponent& source) {
	if (source.path != _clipPath) {
		throw AudioException("Clip path shouldn't be changed during playback");
	}
	alCall(alSourcef, _source, AL_PITCH, source.pitch);
	alCall(alSourcef, _source, AL_GAIN, source.gain);
	_isLooped = source.isLooped;
	alCall(alSourcei, _source, AL_LOOPING, AL_FALSE);
	alCall(alSourcef, _source, AL_MAX_DISTANCE, source.maxDistance);
	alCall(alSourcef, _source, AL_REFERENCE_DISTANCE, source.referenceDistance);
	alCall(alSourcef, _source, AL_ROLLOFF_FACTOR, source.rolloffFactor);
	alCall(alSourcef, _source, AL_CONE_INNER_ANGLE, source.coneInnerAngle);
	alCall(alSourcef, _source, AL_CONE_OUTER_ANGLE, source.coneOuterAngle);
	alCall(alSourcef, _source, AL_CONE_OUTER_GAIN, source.coneOuterGain);
}

void SourcePlayer::update(
	const AudioSourceComponent& source,
	const scene::components::TransformComponent& transform,
	const physics::Velocity& velocity
) {
	update(source);
	alCall(alSource3f, _source, AL_POSITION, transform.position.x, transform.position.y, 0.0f);
	alCall(alSource3f, _source, AL_VELOCITY, velocity.value.x, velocity.value.y, 0.0f);
	if (source.isDirectional) {
		float3 direction = transform.rotation * float3(0.0f, 1.0f, 0.0f);
		alCall(alSource3f, _source, AL_DIRECTION, direction.x, direction.y, 0.0f);
	}
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
	bool isStopped = false;
	switch (alState) {
		case AL_PLAYING: _doNextFrame(); break;
		case AL_PAUSED:	 _continuePlaying(); break;
		case AL_STOPPED:
			isStopped = true;
			break;
		case AL_INITIAL: _startFromBeginning(); break;
		default:		 throw AudioException("Source player has an invalid state");
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
	ALenum alState;
	alCall(alGetSourcei, _source, AL_SOURCE_STATE, &alState);
	if (alState == AL_PAUSED) {
		_continuePlaying();
	}
	alCall(alSourcef, _source, AL_GAIN, 0.0f);
	ALint buffersProcessed, buffersQueued;
	alCall(alGetSourcei, _source, AL_BUFFERS_PROCESSED, &buffersProcessed);
	alCall(alGetSourcei, _source, AL_BUFFERS_QUEUED, &buffersQueued);
	if (buffersProcessed < buffersQueued) {
		return false;
	}
	alCall(alSourceUnqueueBuffers, _source, 4, &_buffers[0]);
	clean();
	return true;
}

void SourcePlayer::rewindPlaying() {
	_cursor = 0;
	_isEndFound = false;
	alCall(alSourcePlay, _source);
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
