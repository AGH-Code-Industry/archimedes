#include <audio/AudioSource.h>
#include <audio/Calls.hpp>
#include <iostream>

namespace arch::audio{

	void AudioSource::updateSource(){
		alCall(alSourcef, source, AL_PITCH, pitch);
		alCall(alSourcef, source, AL_GAIN, gain);
		alCall(alSource3f, source, AL_POSITION, positionX, positionY, 0);
		alCall(alSource3f, source, AL_VELOCITY, velocityX, velocityY, 0);
		int loopingFlag = isLooping ? AL_TRUE : AL_FALSE;
		alCall(alSourcei, source, AL_LOOPING, loopingFlag);
	}

	void AudioSource::loadData(){
		const ALchar* data = clip->getData();
		ALenum format = clip->getFormat();
		ALint size = clip->getSize();
		ALint sampleRate = clip->getSampleRate();
		alCall(alBufferData, buffer, format, data, size, sampleRate);
	}

	AudioSource::AudioSource(const std::string& path, ClipType type, float pitch, float gain, float positionX,
	float positionY, float velocityX, float velocityY, bool isLooping) : pitch(pitch), gain(gain),
	positionX(positionX), positionY(positionY), velocityX(velocityX), velocityY(velocityY), isLooping(isLooping) {
		clip = std::make_unique<Clip>(type, path);
		clip->load();
		alCall(alGenBuffers, 1, &buffer);
		alCall(alGenSources, 1, &source);
		loadData();
		alCall(alSourcei, source, AL_BUFFER, buffer);
		updateSource();
	}

	AudioSource::~AudioSource(){
		alCall(alDeleteSources, 1, &source);
		alCall(alDeleteBuffers, 1, &buffer);
	}

	void AudioSource::play(){
		alCall(alSourcePlay, source);
		ALint state = AL_PLAYING;
		while(state == AL_PLAYING){
			alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
		}
	}
}