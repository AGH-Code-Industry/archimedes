#pragma once
#include <audio/AudioSource.h>
#include <audio/AudioListener.h>

namespace arch::audio {
	///@brief Adds effects to played sounds, modifying parameters of AudioSources and the AudioListener.
	class AudioMixer {
		/// @brief A pointer to AudioSources vector from AudioManager.
		std::vector<AudioSource>* _audioSources{};

		///@brief A pointer to mutex from AudioManager - thanks to this, all modifications are synchronized,
		///especially with updating the OpenAL context.
		std::mutex* _mutex{};

		///@brief A pointer to AudioListener from AudioManager.
		AudioListener* _listener{};

		public:
		///@brief Update pointers to data from the AudioManager.
		///@param audioSources Pointer to the AudioSources vector.
		///@param listener Pointer to the AudioListener.
		///@param mutex Pointer to the mutex.
		///@warning This should be called once before using the object, else using other methods will cause
		///segmentation faults.
		void initialize(std::vector<AudioSource>* audioSources, AudioListener* listener, std::mutex* mutex);

		///@brief Change pitch of an AudioSource.
		///@param index Index to the AudioSource in vector.
		///@param pitch New value of the pitch.
		///@throws AudioException if the index is out of range or the pitch is negative.
		void changeSourcePitch(int index, ALfloat pitch);

		///@brief Change gain of an AudioSource.
		///@param index Index to the AudioSource in vector.
		///@param gain New value of the gain.
		///@throws AudioException if the index is out of range or gain is negative.
		void changeSourceGain(int index, ALfloat gain);

		///@brief Change position of an AudioSource.
		///@param index Index to the AudioSource in vector.
		///@param positionX New value of the X coordinate.
		///@param positionY New value of the Y coordinate.
		///@throws AudioException if the index is out of range.
		void changeSourcePosition(int index, ALfloat positionX, ALfloat positionY);

		///@brief Change velocity of an AudioSource.
		///@param index Index to the AudioSource in vector.
		///@param velocityX New value of the velocity in X axis.
		///@param velocityY New value of the velocity in Y axis.
		///@throws AudioException if the index is out of range.
		void changeSourceVelocity(int index, ALfloat velocityX, ALfloat velocityY);

		///@brief Change a flag for looping of an AudioSource.
		///@param index Index to the AudioSource in vector.
		///@param isLooped New value of a flag for looping.
		///@throws AudioException if the index is out of range.
		void changeSourceIsLooped(int index, bool isLooped);

		///@brief Change master gain of the Listener.
		///@param masterGain New value of the master gain.
		///@throws AudioException if the gain is negative.
		void changeListenerMasterGain(ALfloat masterGain);

		///@brief Change position of the Listener.
		///@param positionX New value of the X coordinate.
		///@param positionY New value of the Y coordinate.
		void changeListenerPosition(ALfloat positionX, ALfloat positionY);

		///@brief Change velocity of the Listener.
		///@param velocityX New value of the velocity in X axis.
		///@param velocityY New value of the velocity in Y axis.
		void changeListenerVelocity(ALfloat velocityX, ALfloat velocityY);

	};
}