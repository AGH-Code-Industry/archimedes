#pragma once
#include <limits>
#include <string>

#include <Logger.h>
#include <math/Math.h>

namespace arch::audio {

/// @brief ECS component used to store all data about a sound source on the scene.
struct AudioSourceComponent {

	/// @brief Path to the audio file.
	std::string path = "";

	/// @brief Tells if the sound's playback has to be looped.
	bool isLooped = false;

	/// @brief Gain modifier of the sound.
	/// @warning Should be positive.
	/// Also, values bigger than 1.0 might be treated as 1.0.
	float gain = 1.0f;

	/// @brief Pitch modifier of the sound.
	float pitch = 1.0f;

	/// @brief Min value of gain after attenuation.
	/// It's always guaranteed for the source.
	/// @warning Should be between 0.0 and 1.0.
	float minGain = 0.0f;

	/// @brief Max value of gain after attenuation.
	/// It's the maximal value permitted for the source.
	/// @warning Should be between 0.0 and 1.0.
	float maxGain = 1.0f;

	///@brief Position in 2D.
	math::float2 position = {0.0f, 0.0f};

	///@brief Velocity vector in 2D.
	math::float2 velocity = {0.0f, 0.0f};

	///@brief A direction vector telling where is the Source looking at.
	///If it's a zero vector, the source isn't directional.
	math::float2 direction = {0.0f, 0.0f};

	///@brief Max distance between the listener and this source to hear the sound.
	///@warning Should be positive.
	float maxDistance = std::numeric_limits<float>::max();

	///@brief During calculating the distance-based attenuation, the distance is compared to this one.
	///@warning Should be positive.
	float referenceDistance = 1.0f;

	///@brief The bigger the factor, the faster the rate at which the gain decreases with distance.
	///@warning Should be positive.
	float rolloffFactor = 1.0f;

	///@brief Inside this range, the gain is constant.
	///It applies only to directional sources.
	float coneInnerAngle = 360.0f;

	///@brief Outside this range, the gain is set to the value in coneOuterGain.
	///Between this range and the one in coneInnerAngle, the gain decreases.
	///@see coneOuterGain
	///@see coneInnerAngle
	float coneOuterAngle = 360.0f;

	///@brief Outside the coneOuterGain range, the gain is set to this.
	///@warning The value should be between 0.0 and 1.0f.
	///@see coneOuterGain
	float coneOuterGain = 0.0f;

	///@brief Set this to true if the SourcePlayer assigned to this
	///shouldn't be marked as removed after the sound ends.
	bool dontRemoveFinished = false;


private:
	///@brief The index of the SourcePlayer assigned to this source.
	///If set to -1, a source isn't assigned.
	int _id = -1;

	friend class AudioManager;
};
} // namespace arch::audio