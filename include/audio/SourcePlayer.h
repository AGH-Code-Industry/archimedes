#pragma once

#include <audio/AudioSource.h>
#include <audio/SoundBank.h>

namespace arch::audio {

/// @brief Each sound played on the game's scene has its own SourcePlayer.
/// The sound can be modified as needed.
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

	/// @brief Pitch modifier of the sound.
	float _pitch = 1.0f;

	/// @brief Gain modifier of the sound.
	float _gain = 1.0f;

	/// @brief Position of the Source on the map.
	/// Used for spatial effects.
	float _positionX = 0.0f, _positionY = 0.0f;

	/// @brief Velocity of the Source on the map.
	/// Used for calculating the Doppler Effect.
	float _velocityX = 0.0f, _velocityY = 0.0f;

	/// @brief Tells if the sound's playback has to be looped.
	bool _isLooped = false;

	/// @brief Passes all the sound's parameters to OpenAL.
	void _updateSoundAttributes();

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

	/// @brief Copies all sound parameters from the AudioSource into this object.
	/// @param source ECS component with info about the sound source.
	/// @throws AudioException if the clip path was modified more than once during the playtime
	/// (but you still can do it after the last sound was stopped).
	void update(const AudioSource& source);

	/// @brief Initializes the _soundBank variable, OpenAL source and OpenAL buffers.
	/// @param soundBank SoundBank responsible for loading the files.
	/// @warning Should be called before using the SourcePlayer, works like a constructor.
	/// @see _soundBank
	void initialize(SoundBank* soundBank);

	/// @brief Destructor. Deletes the OpenAL structures.
	~SourcePlayer();

	/// @brief Controls the playback of the sound. If it hasn't started, start it.
	/// If it's paused, continue it. If it's already playing, update the buffers.
	/// If it's ended, ask the AudioManager to stop it.
	/// @param source AudioSource that this object is assigned to.
	/// @warning If it throws and AudioException saying that the state is invalid,
	/// there is a bug in implementation (and it should be reported).
	void run(AudioSource& source);

	///@brief Stops playing the sound. To do it, OpenAL needs to process all the buffers,
	/// so the function might be called multiple times. In such situations, the sound is muted
	/// and the function ensures that it's still playing, until all the buffers are processed.
	///@returns True if the sound was finally stopped, False if some buffers need to be processed before.
	bool stopPlaying();

	///@brief Pauses playing the sound.
	void pausePlaying();
};

} // namespace arch::audio
