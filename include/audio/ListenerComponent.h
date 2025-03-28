#pragma once

#include <math/Math.h>

namespace arch::audio{

/// @brief ECS component used to store all data about a sound listener on the scene.
struct ListenerComponent {

	///@brief Gain of all listened sounds.
	/// Controls loudness of all the sounds.
	/// @warning Value should be positive.
	/// Also, values bigger than 1.0 might be treated as 1.0.
	float gain = 1.0f;

	///@brief Position in 2D.
	math::float2 position = {0.0f, 0.0f};

	///@brief Velocity vector in 2D.
	math::float2 velocity = {0.0f, 0.0f};

	///@brief A direction vector telling where is the Listener looking at.
	math::float2 orientation = {0.0f, 1.0f};

private:

	///@brief Tells if this listener's data is used by the AudioManager.
	bool _isActive = false;

	friend class AudioManager;
};
}
