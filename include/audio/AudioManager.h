#pragma once

#include <audio/AudioSourceComponent.h>
#include <audio/Listener.h>
#include <audio/SourcePlayer.h>
#include <ecs/Domain.h>

namespace arch::audio {

enum SourceState {
	playing, paused, stopped, removed, unused
};

/// @brief Stores all SourcePlayers on the scene and synchronizes their work.
/// Allows for automatic removal of sounds that stopped playing.
class AudioManager {
	///@brief All SourcePlayers on the scene.
	SourcePlayer _sources[16];

	///@brief Array of flags to tell if a SourcePlayer is in use.
	SourceState _sourceStates[16];

	///@brief Listener object used for calculating relative distance and velocity.
	/// Also controls loudness of all played sounds.
	Listener _listener;

	///@brief Sound bank storing all clips.
	SoundBank* _soundBank;

	///@brief If you want to stop the SourceManager during playing, set it to false.
	bool _isListening = true;

	///@brief Finds a SourcePlayer that is not currently used by any component.
	///@returns Index of the non-used SourcePlayer if it was found, -1 otherwise.
	int _findEmptyPlayer() const;

	///@brief Finds an empty SourcePlayer and assign it to the AudioSourceComponent.
	///@param source ECS component with info about the sound source.
	///@throws AudioException if a non-used SourcePlayer can't be found.
	void _assignSource(AudioSourceComponent* source);


	///@brief Sends current parameters of the Listener to OpenAL.
	void _updateListener();



public:

	///@brief Constructor. Initializes all SourcePlayers.
	///@param soundBank SoundBank object responsible for loading all sounds from the disk.
	///@param domain ECS domain used to access all currently used AudioSourceComponents.
	AudioManager(SoundBank* soundBank);

	///@brief Destructor.
	/// Currently, it does almost nothing (apart from logging).
	~AudioManager();

	///@brief Controls the audio playback that is done by SourcePlayers.
	/// Every frame, each playing SourcePlayer will be updated according to the AudioSourceComponent
	/// it's assigned to. It will ignore all AudioSourceComponents that have their state set to "ignoring"
	/// or clip path set to "".
	///@throws AudioException if there is a non-ignored AudioSourceComponent with assigned SourcePlayer
	/// that doesn't exist. So the index is other than -1 ("SourcePlayer not assigned")
	/// and is outside the SourcePlayers array.
	///@warning This is a blocking function, and it should be used in a separate thread.
	void play();

	///@brief Stops the AudioManager.
	void stop();

	void playSource(AudioSourceComponent* source);

	void pauseSource(const AudioSourceComponent* source);

	void stopSource(const AudioSourceComponent* source);

	void cleanSources(ecs::Domain& domain);

	///@brief Asks the assigned SourcePlayer to get all the sound parameters
	/// from the AudioSourceComponent.
	///@param source ECS component with info about the sound source.
	void updateSource(const AudioSourceComponent* source);
};
} // namespace arch::audio
