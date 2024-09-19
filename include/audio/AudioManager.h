#pragma once
#include <audio/AudioSource.h>
#include <audio/SoundBank.h>
#include <vector>
#include <mutex>


namespace arch::audio {

	/// @brief Stores all AudioSources on the scene and synchronizes their work.
	class AudioManager {

		///@brief All AudioSources on the scene.
		std::vector<AudioSource> _audioSources;

		///@brief Mutex to ensure that only current data will be sent to OpenAL.
		std::mutex _mutex;

		///@brief Index of currently watched AudioSource.
		int _currentIndex = 0;

		///@brief Sound bank storing all clips.
		SoundBank* _soundBank;

		///@brief Check what the specified AudioSource is doing.
		///@param index Index of the AudioSource.
		///@returns SourceState element indicating the state of the AudioSource.
		///@throws AudioException if the index is not valid.
		SourceState _getState(int index);

		///@brief Start playback of specified AudioSource.
		///@param index Index of the AudioSource.
		///@throws AudioException if the index is not valid
		///or the AudioSource is not waiting.
		void _playSource(int index);

		///@brief Send current parameters of the AudioSource to OpenAL.
		///Also load new sound data to buffers. This should be used every audio frame.
		///@param index Index of the AudioSource.
		///@throws AudioException if the index is not valid
		///or the AudioSource is not playing.
		void _updateSource(int index);

		///@brief Check if current iteration reached the end of the AudioSources' vector.
		///@return True if the end was reached, false otherwise.
		bool _vectorEndReached();

		public:

		///@brief Max number of stored AudioSources.
		const int maxSources = 16;

		///@brief If you want to stop the AudioManager during playing, set it to false.
		//TODO: there should be a better way (a new method maybe?)
		bool isListening = true;

		///@brief Constructor.
		///@param soundBank Sound bank storing all the clips.
		AudioManager(SoundBank* soundBank);

		///@brief Destructor.
		///Currently, it does almost nothing (apart from logging).
		~AudioManager();

		///@brief Play all the AudioSources.
		///Every frame, each playing AudioSource will be updated.
		///AudioSources that stopped playing will be removed from the AudioManager.
		///@warning This is a blocking function, and it should be used in a separate thread.
		void play();

		///@brief Add a new AudioSource.
		/// @param path Path of the sound file.
		/// @param pitch Pitch modifier of the sound.
		/// @param gain Gain modifier of the sound.
		/// @param positionX X coordinate of the position on the map.
		/// @param positionY Y coordinate of the position on the map.
		/// @param velocityX Source's velocity on the X axis.
		/// @param velocityY Source's velocity on the Y axis.
		/// @param isLooped Tells if the sound's playback has to be looped.
		/// @throws AudioException if the number of AudioSources is equal to maxSources.
		/// @see maxSources
		void addSource(const std::string& path, float pitch = 1.0f, float gain = 1.0f,
					float positionX = 0.0f,float positionY = 0.0f,float velocityX = 0.0f,
					float velocityY = 0.0f,bool isLooped = false);

		///@brief Remove an AudioSource.
		///@param index Index of the AudioSource.
		///@throws AudioException if the index is not valid.
		void removeSource(int index);

		///@brief Pause playing the sound by an AudioSource.
		///@param index Index of the AudioSource.
		///@throws AudioException if the index is not valid.
		void pauseSource(int index);

		///@brief Continue playing the sound by an AudioSource.
		///@param index Index of the AudioSource.
		///@throws AudioException if the index is not valid.
		void continueSource(int index);
	};
}