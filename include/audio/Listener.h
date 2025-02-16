#pragma once

namespace arch::audio {
///@brief Stores data about the listener of all played sounds.
/// Used for calculating effects of relative speed and distance,
/// also controls how loud all the sounds will be.
class Listener {
public:
	///@brief Gain of all listened sounds.
	/// Controls loudness of all the sounds.
	float masterGain;

	///@brief Position of the Listener on the map.
	/// Used for calculating spatial effects.
	float positionX, positionY;

	///@brief Velocity of the Listener.
	/// Used for calculating the Doppler effect.
	float velocityX, velocityY;

	///@brief Constructor. Calls update at startup.
	///@param masterGain Gain of all listened sounds.
	///@param positionX X coordinate of the position.
	///@param positionY Y coordinate of the position.
	///@param velocityX velocity on the X axis.
	///@param velocityY velocity on the Y axis.
	explicit Listener(
		float masterGain = 1.0f,
		float positionX = 0.0f,
		float positionY = 0.0f,
		float velocityX = 0.0f,
		float velocityY = 0.0f
	);

	///@brief Sends the parameters to the OpenAL context.
	///@warning Should be called each audio frame.
	void update();
};
} // namespace arch::audio
