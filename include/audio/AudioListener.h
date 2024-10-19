#pragma once
#include <mutex>
#include <AL/al.h>

namespace arch::audio {
class AudioListener {
	public:
	ALfloat masterGain;
	ALfloat positionX, positionY;
	ALfloat velocityX, velocityY;

	AudioListener(ALfloat masterGain=1.0f, ALfloat positionX=0.0f, ALfloat positionY=0.0f,
					ALfloat velocityX=0.0f, ALfloat velocityY=0.0f);

	void update();
};
}