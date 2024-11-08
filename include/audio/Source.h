#pragma once

#include "SourceComponent.h"
#include <AL/al.h>
#include <audio/Clip.h>
#include <audio/SoundBank.h>

namespace arch::audio{

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

		void activate(SoundBank* soundBank, SourceComponent& component);

		void deactivate();

		void play();

		void update(SourceComponent& component);

		void stop();

		void pausePlaying();

		void continuePlaying();
	};

}