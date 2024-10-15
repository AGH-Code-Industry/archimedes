#pragma once
#include <mutex>
#include <AL/al.h>

namespace arch::audio {
class AudioListener {
	ALfloat _masterGain;
	ALfloat _positionX, _positionY;
	ALfloat _velocityX, _velocityY;

	void _changeMasterGain(ALfloat masterGain, std::mutex* mutex);
	void _changePosition(ALfloat positionX, ALfloat positionY, std::mutex* mutex);
	void _changeVelocity(ALfloat velocityX, ALfloat velocityY, std::mutex* mutex);

	public:

	AudioListener(ALfloat masterGain=1.0f, ALfloat positionX=0.0f, ALfloat positionY=0.0f,
					ALfloat velocityX=0.0f, ALfloat velocityY=0.0f);

	void update();
};
}