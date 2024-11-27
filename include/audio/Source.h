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

		/// @brief True if end of the sound file was reached.
		bool _isEndFound = false;

		/// @brief Pitch modifier of the sound.
		ALfloat _pitch = 1.0f;

		/// @brief Gain modifier of the sound.
		ALfloat _gain = 1.0f;

		/// @brief Position of the Source on the map.
		/// Used for spatial effects.
		ALfloat _positionX = 0.0f, _positionY = 0.0f;

		/// @brief Velocity of the Source on the map.
		/// Used for calculating the Doppler Effect.
		ALfloat _velocityX = 0.0f, _velocityY = 0.0f;

		/// @brief Tells if the sound's playback has to be looped.
		bool _isLooped = false;

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

		void _doNextFrame(SourceComponent& component);

		void _continuePlaying();

		void _startFromBeginning();


		public:

		void update(const SourceComponent& component);

		void initialize(SoundBank* soundBank);

		~Source();

		/// @brief Play the sound from the beginning.
		void play(SourceComponent& component);

		void stopPlaying();

		void pausePlaying();
	};

}