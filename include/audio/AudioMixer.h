#pragma once
#include <audio/AudioSource.h>
#include <audio/AudioListener.h>

namespace arch::audio {
	class AudioMixer {
		std::vector<AudioSource>* _audioSources{};
		std::mutex* _mutex{};
		AudioListener* _listener{};

		public:
		void initialize(std::vector<AudioSource>* audioSources, AudioListener* listener, std::mutex* mutex);

		void changeSourcePitch(int index, ALfloat pitch);
		void changeSourceGain(int index, ALfloat gain);
		void changeSourcePosition(int index, ALfloat positionX, ALfloat positionY);
		void changeSourceVelocity(int index, ALfloat velocityX, ALfloat velocityY);
		void changeSourceIsLooped(int index, bool isLooped);

		void changeListenerMasterGain(ALfloat masterGain);
		void changeListenerPosition(ALfloat positionX, ALfloat positionY);
		void changeListenerVelocity(ALfloat velocityX, ALfloat velocityY);

	};
}