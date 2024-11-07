#pragma once
#include <audio/Source.h>
#include <audio/Listener.h>

namespace arch::audio {
	///@brief Adds effects to played sounds, modifying parameters of Sources and the Listener.
	class Mixer {
		/// @brief A pointer to Sources vector from SourceManager.
		std::vector<Source>* _sources{};

		///@brief A pointer to mutex from SourceManager - thanks to this, all modifications are synchronized,
		///especially with updating the OpenAL context.
		std::mutex* _mutex{};

		///@brief A pointer to Listener from SourceManager.
		Listener* _listener{};

		public:
		///@brief Update pointers to data from the SourceManager.
		///@param sources Pointer to the Sources vector.
		///@param listener Pointer to the Listener.
		///@param mutex Pointer to the mutex.
		///@warning This should be called once before using the object, else using other methods will cause
		///segmentation faults.
		void initialize();

		///@brief Change pitch of a Source.
		///@param index Index to the Source in vector.
		///@param pitch New value of the pitch.
		///@throws AudioException if the index is out of range or the pitch is negative.
		void changeSourcePitch(int index, ALfloat pitch);

		///@brief Change gain of a Source.
		///@param index Index to the Source in vector.
		///@param gain New value of the gain.
		///@throws AudioException if the index is out of range or gain is negative.
		void changeSourceGain(int index, ALfloat gain);

		///@brief Change position of a Source.
		///@param index Index to the Source in vector.
		///@param positionX New value of the X coordinate.
		///@param positionY New value of the Y coordinate.
		///@throws AudioException if the index is out of range.
		void changeSourcePosition(int index, ALfloat positionX, ALfloat positionY);

		///@brief Change velocity of a Source.
		///@param index Index to the Source in vector.
		///@param velocityX New value of the velocity in X axis.
		///@param velocityY New value of the velocity in Y axis.
		///@throws AudioException if the index is out of range.
		void changeSourceVelocity(int index, ALfloat velocityX, ALfloat velocityY);

		///@brief Change a flag for looping of a Source.
		///@param index Index to the Source in vector.
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