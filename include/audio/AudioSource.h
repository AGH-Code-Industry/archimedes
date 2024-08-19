#pragma once
#include <AL/al.h>
#include <audio/Clip.h>
#include <audio/SoundBank.h>
#include <memory>

namespace arch::audio{
	/// @brief Each sound played on the game's scene has its own AudioSource.
	/// The sound can be modified as needed.
	class AudioSource{

		/// @brief Position in the audio file.
		std::size_t _cursor = 0;

		/// @brief Buffer loaded by Clip when AudioSource is streaming the sound.
		std::vector<ALshort> _loadingBuffer;

		/// @brief SoundBank responsible for loading audio data from files.
		SoundBank& _soundBank;

		/// @brief Pass all the sound's parameters to OpenAL.
		void _updateSoundAttributes() const;

		/// @brief Load first 4 buffers with audio.
		/// @returns True if the end of sound was reached, false otherwise.
		bool _initiallyLoadSound();

		/// @brief Replace buffers processed by OpenAL with ones with new data.
		/// @returns True if the end of sound was reached, false otherwise.
		bool _loadSound();

		/// @brief Reserve enough data in the _loadingBuffer and fill it with zeroes.
		/// @see _loadingBuffer
		void _prepareLoadingBuffer();

		public:

		/// @brief Pitch modifier of the sound.
		ALfloat pitch;

		/// @brief Gain modifier of the sound.
		ALfloat gain;

		/// @brief Position of the AudioSource on the map.
		/// Used for spatial effects.
		ALfloat positionX, positionY;

		/// @brief Velocity of the AudioSource on the map.
		/// Used for calculating the Doppler Effect.
		ALfloat velocityX, velocityY;

		/// @brief Tells if the sound's playback has to be looped.
		bool isLooped;

		/// @brief OpenAL source index.
		/// Value returned by alGenSources().
		ALuint source;

		/// @brief OpenAL buffers' indexes.
		/// Values returned by alGenBuffers().
		ALuint buffers[4];

		/// @brief Path to the audio file.
		std::string clipPath;

		/// @brief Tells if the playback is currently paused.
		bool isPaused = false;

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
		AudioSource(SoundBank& soundBank, const std::string& path, float pitch=1.0f, float gain=1.0f,
					float positionX=0.0f, float positionY=0.0f, float velocityX=0.0f,
					float velocityY=0.0f, bool isLooped=false);

		/// @brief Destructor.
		~AudioSource();

		/// @brief Start the playback.
		/// It's a blocking function, so it should be called in a separate thread.
		void play();
	};

}