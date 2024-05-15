#pragma once
#include <AL/al.h>
#include <audio/Clip.h>
#include <memory>

namespace arch::audio{
	class AudioSource{
		std::size_t _cursor = 0;
		std::vector<ALshort> _loadingBuffer;

		void _updateSoundAttributes() const;
		bool _initiallyLoadSound();
		bool _loadSound();
		void _prepareLoadingBuffer();

		public:
		ALfloat pitch, gain, positionX, positionY, velocityX, velocityY;
		bool isLooped;
		ALuint source, buffers[4];
		std::unique_ptr<Clip> clip;
		bool isPaused = false;
		AudioSource(const std::string& path, float pitch=1.0f, float gain=1.0f,
					float positionX=0.0f, float positionY=0.0f, float velocityX=0.0f,
					float velocityY=0.0f, bool isLooped=false);
		~AudioSource();
		void play();
	};

}