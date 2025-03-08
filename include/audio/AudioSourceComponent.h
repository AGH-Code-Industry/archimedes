#pragma once
#include <string>

#include <Logger.h>

namespace arch::audio {

/// @brief Tells the AudioManager what to do with the AudioSource.
enum SourceAction {
	play,
	pause,
	stop
};

/// @brief ECS component used to store all data about a sound source on the scene.
/// It also allows to specify what the AudioManager should do with it.
struct AudioSourceComponent {

	/// @brief Path to the audio file.
	std::string path = "";

	/// @brief Gain modifier of the sound.
	float gain = 1.0f;

	/// @brief Tells if the sound's playback has to be looped.
	bool isLooped = false;

	/// @brief Pitch modifier of the sound.
	float pitch = 1.0f;

	/// @brief Position of the AudioSource on the map.
	/// Used for spatial effects.
	float positionX = 0.0f, positionY = 0.0f;

	/// @brief Velocity of the AudioSource on the map.
	/// Used for calculating the Doppler Effect.
	float velocityX = 0.0f, velocityY = 0.0f;
};
} // namespace arch::audio