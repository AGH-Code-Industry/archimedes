#pragma once
#include <mutex>
#include <AL/al.h>

namespace arch::audio {
///@brief Stores data about the listener of all played sounds.
///Used for calculating effects of relative speed and distance,
///also controls how loud all the sounds will be.
class AudioListener {
	public:
	///@brief Gain of all listened sounds.
	///Controls loudness of all the sounds.
	ALfloat masterGain;

	///@brief Position of the Listener on the map.
	///Used for calculating spatial effects.
	ALfloat positionX, positionY;

	///@brief Velocity of the Listener.
	///Used for calculating the Doppler effect.
	ALfloat velocityX, velocityY;

	///@brief Constructor. Calls update at startup.
	///@param masterGain Gain of all listened sounds.
	///@param positionX X coordinate of the position.
	///@param positionY Y coordinate of the position.
	///@param velocityX velocity on the X axis.
	///@param velocityY velocity on the Y axis.
	AudioListener(ALfloat masterGain=1.0f, ALfloat positionX=0.0f, ALfloat positionY=0.0f,
					ALfloat velocityX=0.0f, ALfloat velocityY=0.0f);

	///@brief Sends the parameters to the OpenAL context.
	///@warning Should be called each audio frame.
	void update();
};
}