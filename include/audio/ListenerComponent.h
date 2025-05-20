#pragma once

namespace arch::audio{

/// @brief ECS component used to store all data about a sound listener on the scene.
struct ListenerComponent {

	///@brief Gain of all listened sounds.
	/// Controls loudness of all the sounds.
	/// @warning Value should be positive.
	/// Also, values bigger than 1.0 might be treated as 1.0.
	float gain = 1.0f;

private:

	///@brief Tells if this listener's data is used by the AudioManager.
	bool _isActive = false;

	friend class AudioManager;
};
}
