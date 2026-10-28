#pragma once

#include <archimedes/audio/AudioSourceComponent.h>
#include <archimedes/audio/SoundBank.h>
#include <archimedes/physics/components/RigidBodyComponent.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::audio {

/// @brief Each sound played on the game's scene has its own SourcePlayer.
/// Each SourcePlayer controls playing a particular sound and sends needed
/// data to OpenAL.
class SourcePlayer {

	/// @brief SoundBank responsible for loading audio data from files.
	SoundBank* _soundBank{};

	/// @brief OpenAL source index.
	/// Value returned by alGenSources().
	unsigned int _source{};

	/// @brief Path to the audio file.
	std::string _clipPath;

	/// @brief Tells if the sound's playback has to be looped.
	bool _isLooped = false;

	/// @brief Continues playing a sound that was paused before.
	void _continuePlaying();

	/// @brief Starts playing the sound from beginning.
	void _startFromBeginning();

	void _updateParamsFromComponent(const AudioSourceComponent& source);
	void _loadClip(std::string_view path);

public:

	/// @brief Copies all sound parameters from the AudioSourceComponent into this object.
	/// @param source ECS component with info about the sound.
	/// @param transform ECS component with info about the source position.
	/// @param moveable ECS component with info about velocity.
	/// @throws AudioException if the clip path was modified during the playback.
	void update(
		const AudioSourceComponent& source,
		const scene::components::TransformComponent& transform,
		const physics::RigidBodyComponent& moveable
	);

	/// @brief Copies all sound parameters from the AudioSourceComponent into this object.
	/// Doesn't use spatial data.
	/// @param source ECS component with info about the sound.
	/// @throws AudioException if the clip path was modified during the playback.
	void update(const AudioSourceComponent& source);


	/// @brief Initializes the _soundBank variable, OpenAL source and OpenAL buffers.
	/// @param soundBank SoundBank responsible for loading the files.
	/// @warning Should be called before using the SourcePlayer.
	/// @see _soundBank
	void initialize(SoundBank* soundBank);

	/// @brief Destructor. Deletes the OpenAL structures.
	~SourcePlayer();

	///@brief Stops playing the sound
	void stopPlaying();

	///@brief Pauses playing the sound.
	void pausePlaying();

	///@brief Rewinds the playing sound.
	///@warning It's advised to use it only when the source is playing and
	/// it won't be automatically removed when it ends.
	void rewindPlaying();

	///@brief Deletes the clip path.
	/// Sets all parameters to default values.
	void clean();
};

} // namespace arch::audio
