#include <audio/AudioSource.h>
#include <audio/Calls.hpp>

namespace arch::audio{

	void AudioSource::_updateSoundAttributes() const{
		alCall(alSourcef, source, AL_PITCH, pitch);
		alCall(alSourcef, source, AL_GAIN, gain);
		alCall(alSource3f, source, AL_POSITION, positionX, positionY, 0);
		alCall(alSource3f, source, AL_VELOCITY, velocityX, velocityY, 0);
		alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
	}

	bool AudioSource::_initiallyLoadSound() {
		ALenum format = clip->getFormat();
		std::size_t bufferElements = clip->getBufferElements();
		ALint sampleRate = clip->getSampleRate();
		bool isEndFound = false;
		for(int i=0; i<4; i++) {
			isEndFound |= clip->fillBuffer(_loadingBuffer, _cursor, isLooped);
			alCall(alBufferData, buffers[i], format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);
		}
		return isEndFound;
	}

	bool AudioSource::_loadSound() {
		ALenum format = clip->getFormat();
		std::size_t bufferElements = clip->getBufferElements();
		ALint sampleRate = clip->getSampleRate();

		ALint buffersProcessed = 0;
		alCall(alGetSourcei, source, AL_BUFFERS_PROCESSED, &buffersProcessed);

		if(buffersProcessed <= 0) {
			return false;
		}

		bool isEndFound = false;
		for(int i=0; i<buffersProcessed; i++) {
			ALuint buffer;
			alCall(alSourceUnqueueBuffers, source, 1, &buffer);
			isEndFound |= clip->fillBuffer(_loadingBuffer, _cursor, isLooped);
			alCall(alBufferData, buffer, format, _loadingBuffer.data(), bufferElements * sizeof(short), sampleRate);
			alCall(alSourceQueueBuffers, source, 1, &buffer);
		}
		return isEndFound;
	}

	void AudioSource::_prepareLoadingBuffer() {
		const std::size_t bufferElements = clip->getBufferElements();
		_loadingBuffer.reserve(bufferElements);
		for(int i=0; i<bufferElements; i++) {
			_loadingBuffer.push_back(0);
		}
	}

	AudioSource::AudioSource(const std::string& path, float pitch, float gain, float positionX,
	float positionY, float velocityX, float velocityY, bool isLooped) : pitch(pitch), gain(gain),
	positionX(positionX), positionY(positionY), velocityX(velocityX), velocityY(velocityY), isLooped(isLooped) {
		clip = std::make_unique<Clip>(path);
		clip->load();
		alCall(alGenBuffers, 4, &buffers[0]);
		alCall(alGenSources, 1, &source);
		_prepareLoadingBuffer();
	}

	AudioSource::~AudioSource(){
		alCall(alDeleteSources, 1, &source);
		alCall(alDeleteBuffers, 4, &buffers[0]);
	}

	void AudioSource::play(){
		_updateSoundAttributes();
		bool isEndFound = _initiallyLoadSound();
		alCall(alSourceQueueBuffers, source, 4, &buffers[0]);

		alCall(alSourcePlay, source);
		ALint state = AL_PLAYING;
		do {
			alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
			_updateSoundAttributes();
			if(isLooped or not isEndFound) {
				isEndFound = _loadSound();
			}
			if(isPaused) {
				alCall(alSourcePause, source);
				while(isPaused) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
				alCall(alSourcePlay, source);
			}

		}while(state == AL_PLAYING);
		alCall(alSourceStop, source);
	}
}