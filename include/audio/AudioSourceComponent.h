#pragma once
#include <limits>
#include <string>

#include <Logger.h>
#include <math/Math.h>

namespace arch::audio {

/// @brief ECS component used to store all data about a sound source on the scene.
/// It also allows to specify what the AudioManager should do with it.
struct AudioSourceComponent {

	/// @brief Path to the audio file.
	std::string path = "";

	/// @brief Tells if the sound's playback has to be looped.
	bool isLooped = false;

	/// @brief Gain modifier of the sound.
	float gain = 1.0f;

	/// @brief Pitch modifier of the sound.
	float pitch = 1.0f;

	float minGain = 0.0f, maxGain = 1.0f;


	math::float2 position = {0.0f, 0.0f};
	math::float2 velocity = {0.0f, 0.0f};
	math::float2 direction = {0.0f, 0.0f};

	float maxDistance = std::numeric_limits<float>::max();
	float referenceDistance = 1.0f;
	float rolloffFactor = 1.0f;

	float coneInnerAngle = 360.0f, coneOuterAngle = 360.0f;
	float coneOuterGain = 0.0f;


private:
	int _id = -1;

	friend class AudioManager;
};
} // namespace arch::audio