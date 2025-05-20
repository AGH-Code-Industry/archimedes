#pragma once

#include <math/Math.h>

namespace arch::audio{
struct ListenerComponent {

	///@brief Gain of all listened sounds.
	/// Controls loudness of all the sounds.
	float gain = 1.0f;

	math::float2 position = {0.0f, 0.0f};
	math::float2 velocity = {0.0f, 0.0f};

private:
	bool _isActive = false;

	friend class AudioManager;
};
}
