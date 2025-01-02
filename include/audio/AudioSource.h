#pragma once
#include <string>

#include <AL/al.h>
#include <Logger.h>

namespace arch::audio {

/// @brief Tells the AudioManager what to do with the AudioSource.
enum SourceState {
	ignoring,
	playing,
	pausing,
	stopping
};

/// @brief ECS component used to store all data about a sound source on the scene.
/// It also allows to specify what the AudioManager should do with it.
struct AudioSource {
	/// @brief Pitch modifier of the sound.
	ALfloat pitch = 1.0f;

	/// @brief Gain modifier of the sound.
	ALfloat gain = 1.0f;

	/// @brief Position of the AudioSource on the map.
	/// Used for spatial effects.
	ALfloat positionX = 0.0f, positionY = 0.0f;

	/// @brief Velocity of the AudioSource on the map.
	/// Used for calculating the Doppler Effect.
	ALfloat velocityX = 0.0f, velocityY = 0.0f;

	/// @brief Tells if the sound's playback has to be looped.
	bool isLooped = false;

	/// @brief Index of the SourcePlayer assigned to the AudioSource.
	/// The SourcePlayer will send the sound's data to OpenAL source, effectively
	/// playing it. The default value is -1, meaning that the AudioManager
	/// hadn't assigned the SourcePlayer yet.
	/// @warning Should be modified ONLY by the AudioManager.
	int sourceIndex = -1;

	/// @brief Path to the audio file.
	std::string path = "";

	/// @brief Plays the sound.
	/// Works only if the sound is currently ignored or paused.
	void play();

	/// @brief Pauses the sound.
	/// Works only if the sound is played.
	void pause();

	/// @brief Stops the sound.
	/// Works only if the sound is played or paused.
	void stop();

	/// @brief Starts ignoring the sound. This means that the AudioManager won't use the sound.
	/// Works only if the sound is stopped.
	/// @warning It's automatically called by the AudioManager after stopping the sound,
	/// so you shouldn't need to use it.
	void ignore();

	/// @brief Returns current state of the AudioSource.
	/// It tells what the AudioManager will do with the sound.
	/// @returns Current value of the _state variable, which can't
	/// be modified directly.
	/// @see _state
	SourceState getState() const;

private:
	/// @brief Tells what the AudioManager should do with the sound.
	/// No AudioSource will be played until the user decides to do so.
	SourceState _state = ignoring;
};
} // namespace arch::audio
