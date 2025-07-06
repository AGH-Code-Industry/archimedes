#pragma once

#include <audio/AudioSourceComponent.h>
#include <audio/SoundBank.h>
#include <physics/components/RigidBodyComponent.h>
#include <scene/components/TransformComponent.h>

namespace arch::audio {

/// @brief Each sound played on the game's scene has its own SourcePlayer.
/// Each SourcePlayer controls playing a particular sound and sends needed
/// data to OpenAL.
class SourcePlayer {
	/// @brief Position in the audio file.
	std::size_t _cursor = 0;

	/// @brief Buffer loaded by Clip when SourcePlayer is streaming the sound.
	std::vector<short> _loadingBuffer;

	/// @brief SoundBank responsible for loading audio data from files.
	SoundBank* _soundBank{};

	/// @brief OpenAL source index.
	/// Value returned by alGenSources().
	unsigned int _source{};

	/// @brief OpenAL buffers' indexes.
	/// Values returned by alGenBuffers().
	unsigned int _buffers[4] = {};
	// TODO: one day this number of buffers might be not sufficient
	// (because now all Sources have one common thread)

	/// @brief Path to the audio file.
	std::string _clipPath;

	/// @brief True if end of the sound file was reached.
	bool _isEndFound = false;

	/// @brief Tells if the sound's playback has to be looped.
	bool _isLooped = false;

	/// @brief Loads first 4 buffers with audio.
	/// @returns True if the end of sound was reached, false otherwise.
	bool _initiallyLoadSound();

	/// @brief Replaces buffers processed by OpenAL with ones with new data.
	/// @returns True if the end of sound was reached, false otherwise.
	bool _loadSound();

	/// @brief Reserves enough data in the _loadingBuffer and fill it with zeroes.
	/// @see _loadingBuffer
	void _prepareLoadingBuffer();

	/// @brief Loads the sound's data to buffers if the sound is looped or the end of the file
	/// hasn't been reached.
	void _doNextFrame();

	/// @brief Continues playing a sound that was paused before.
	void _continuePlaying();

	/// @brief Starts playing the sound from beginning.
	void _startFromBeginning();

public:

	/// @brief Copies all sound parameters from the AudioSourceComponent into this object.
	/// @param source ECS component with info about the sound.
	/// @param transform ECS component with info about the source position.
	/// @param moveable ECS component with info about velocity.
	/// @throws AudioException if the clip path was modified during the playback.
	void update(const AudioSourceComponent& source, const scene::components::TransformComponent& transform,
				const physics::RigidBodyComponent& moveable);

	/// @brief Copies all sound parameters from the AudioSourceComponent into this object.
	/// Doesn't use spatial data.
	/// @param source ECS component with info about the sound.
	/// @throws AudioException if the clip path was modified during the playback.
	void update(const AudioSourceComponent& source);

	//TODO add clean method to use after stop

	/// @brief Initializes the _soundBank variable, OpenAL source and OpenAL buffers.
	/// @param soundBank SoundBank responsible for loading the files.
	/// @warning Should be called before using the SourcePlayer.
	/// @see _soundBank
	void initialize(SoundBank* soundBank);

	/// @brief Destructor. Deletes the OpenAL structures.
	~SourcePlayer();

	/// @brief Controls the playback of the sound. If it hasn't started, start it.
	/// If it's paused, continue it. If it's already playing, update the buffers.
	/// @returns True if the sound ended on its own. False otherwise.
	/// @warning If it throws and AudioException saying that the state is invalid,
	/// there is a bug in implementation (and it should be reported).
	bool run();

	///@brief Stops playing the sound. To do it, OpenAL needs to process all the buffers,
	/// so the function might be called multiple times. In such situations, the sound is muted
	/// and the function ensures that it's still playing, until all the buffers are processed.
	///@returns True if the sound was finally stopped, False if some buffers need to be processed before.
	bool stopPlaying();

	///@brief Pauses playing the sound.
	void pausePlaying();

	///@brief Rewinds the playing sound.
	///@warning It's advised to use it only when the source is playing and
	///it won't be automatically removed when it ends.
	void rewindPlaying();

	///@brief Sets the new clip path.
	///@param clipPath clip path.
	void setClipPath(const std::string& clipPath);

	///@brief Deletes the clip path.
	///Sets all parameters to default values.
	void clean();
};

} // namespace arch::audio
