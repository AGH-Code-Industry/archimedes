#pragma once

namespace arch::audio{
struct ListenerComponent {

	///@brief Gain of all listened sounds.
	/// Controls loudness of all the sounds.
	float masterGain = 1.0f;

	///@brief Position of the Listener on the map.
	/// Used for calculating spatial effects.
	float positionX = 0.0f, positionY = 0.0f;

	///@brief Velocity of the Listener.
	/// Used for calculating the Doppler effect.
	float velocityX = 0.0f, velocityY = 0.0f;
};
}
