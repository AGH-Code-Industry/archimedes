#pragma once

namespace arch::audio{
struct ListenerComponent {

	///@brief Gain of all listened sounds.
	/// Controls loudness of all the sounds.
	float masterGain;

	///@brief Position of the Listener on the map.
	/// Used for calculating spatial effects.
	float positionX, positionY;

	///@brief Velocity of the Listener.
	/// Used for calculating the Doppler effect.
	float velocityX, velocityY;
};
}
