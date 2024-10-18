#pragma once
#include <mutex>
#include <AL/al.h>

namespace arch::audio {
class AudioListener {
	ALfloat _masterGain;
	ALfloat _positionX, _positionY;
	ALfloat _velocityX, _velocityY;

	public:

	AudioListener(ALfloat masterGain=1.0f, ALfloat positionX=0.0f, ALfloat positionY=0.0f,
					ALfloat velocityX=0.0f, ALfloat velocityY=0.0f);

	void changeMasterGain(ALfloat masterGain, std::mutex* mutex);
	void changePosition(ALfloat positionX, ALfloat positionY, std::mutex* mutex);
	void changeVelocity(ALfloat velocityX, ALfloat velocityY, std::mutex* mutex);

	void update();
};
}