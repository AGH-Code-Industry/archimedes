#pragma once

#include "SourceComponent.h"
#include <AL/al.h>
#include <audio/Clip.h>
#include <audio/SoundBank.h>

namespace arch::audio{

	// /// @brief An enum used to tell what the Source is now doing.
	// enum class SourceState {
	// 	waiting, ///< waiting to play a sound
	// 	playing, ///< playing a sound
	// 	stopped, ///< stopped permanently
	// 	paused ///< stopped until continuation
	// };

	/// @brief Each sound played on the game's scene has its own Source.
	/// The sound can be modified as needed.
	class Source{

		/// @brief Position in the audio file.
		std::size_t _cursor = 0;

		/// @brief Buffer loaded by Clip when Source is streaming the sound.
		std::vector<ALshort> _loadingBuffer;

		/// @brief SoundBank responsible for loading audio data from files.
		SoundBank* _soundBank{};

		/// @brief OpenAL source index.
		/// Value returned by alGenSources().
		ALuint _source{};

		/// @brief OpenAL buffers' indexes.
		/// Values returned by alGenBuffers().
		ALuint _buffers[4] = {};
		// TODO: one day this number of buffers might be not sufficient
		// (because now all Sources have one common thread)

		/// @brief Path to the audio file.
		std::string _clipPath;

		// /// @brief Tells what the Source is now doing.
		// SourceState _state = SourceState::waiting;

		/// @brief True if end of the sound file was reached
		bool _isEndFound = false;

		/// @brief Pass all the sound's parameters to OpenAL.
		void _updateSoundAttributes();

		/// @brief Load first 4 buffers with audio.
		/// @returns True if the end of sound was reached, false otherwise.
		bool _initiallyLoadSound();

		/// @brief Replace buffers processed by OpenAL with ones with new data.
		/// @returns True if the end of sound was reached, false otherwise.
		bool _loadSound();

		/// @brief Reserve enough data in the _loadingBuffer and fill it with zeroes.
		/// @see _loadingBuffer
		void _prepareLoadingBuffer();

		void _updateFromComponent(SourceComponent& component);

		public:

		bool isPlaying = false;

		bool isActive = false;

		/// @brief Pitch modifier of the sound.
		ALfloat pitch = 1.0f;

		/// @brief Gain modifier of the sound.
		ALfloat gain = 1.0f;

		/// @brief Position of the Source on the map.
		/// Used for spatial effects.
		ALfloat positionX = 0.0f, positionY = 0.0f;

		/// @brief Velocity of the Source on the map.
		/// Used for calculating the Doppler Effect.
		ALfloat velocityX = 0.0f, velocityY = 0.0f;

		/// @brief Tells if the sound's playback has to be looped.
		bool isLooped = false;

		/// @brief Constructor.
		/// @param soundBank SoundBank responsible for loading audio data from files.
		/// @param path Path of the sound file.
		/// @param pitch Pitch modifier of the sound.
		/// @param gain Gain modifier of the sound.
		/// @param positionX X coordinate of the position on the map.
		/// @param positionY Y coordinate of the position on the map.
		/// @param velocityX Source's velocity on the X axis.
		/// @param velocityY Source's velocity on the Y axis.
		/// @param isLooped Tells if the sound's playback has to be looped.

		///@brief Initialize OpenAL buffers and the source, initially load the sound.
		///@warning Should be always used before playing.
		void activate(SoundBank* soundBank);

		///@brief Clear OpenAL buffers and the source.
		///@warning Should be used when the Source is destroyed.
		void deactivate();

		// ///@brief Getter for the _state property.
		// ///@see _state
		// SourceState getState();

		///@brief Starts the playback.
		///@throws AudioException if the Source is not waiting.
		void play();

		///@brief Sends all sound data to the OpenAL context.
		///@warning Should be called each audio frame.
		///@throws AudioException if the Source is not playing.
		void update(SourceComponent& component);

		///@brief Permanently stops playing the sound.
		///@throws AudioException if the Source is not playing.
		void stop();

		///@brief Pauses the Source until you use continuePlaying().
		///@see continuePlaying
		///@throws AudioException if the Source is not playing.
		void pausePlaying();

		///@brief Continues playing the sound, if the Source is paused.
		///@see pausePlaying
		///@throws AudioException if the Source is not paused.
		void continuePlaying();
	};

}