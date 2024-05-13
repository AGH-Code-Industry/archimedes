#pragma once
#include <AL/al.h>
#include <audio/Clip.h>
#include <memory>

namespace arch::audio{
	class AudioSource{
		void _updateSource();
		void _loadData();

		public:
		ALfloat pitch, gain, positionX, positionY, velocityX, velocityY;
		bool isLooping;
		ALuint source, buffer;
		std::unique_ptr<Clip> clip;
		AudioSource(const std::string& path, float pitch=1.0f, float gain=1.0f,
					float positionX=0.0f, float positionY=0.0f, float velocityX=0.0f,
					float velocityY=0.0f, bool isLooping=false);
		~AudioSource();
		void play();
	};

}