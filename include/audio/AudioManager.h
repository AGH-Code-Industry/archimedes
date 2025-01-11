#pragma once

#include <audio/AudioSourceComponent.h>
#include <audio/Listener.h>
#include <audio/SourcePlayer.h>
#include <ecs/Domain.h>

namespace arch::audio {

/// @brief Stores all SourcePlayers on the scene and synchronizes their work.
/// Allows for automatic removal of sounds that stopped playing.
class AudioManager {
	///@brief ECS Domain object allowing to access all AudioSourceComponents
	ecs::Domain* _domain;

	///@brief All SourcePlayers on the scene.
	SourcePlayer _sources[16];

	///@brief Array of flags to tell if a SourcePlayer is in use.
	bool _sourceUsed[16] = { false };

	///@brief Listener object used for calculating relative distance and velocity.
	/// Also controls loudness of all played sounds.
	Listener _listener;

	///@brief Sound bank storing all clips.
	SoundBank* _soundBank;

	///@brief Mutex used to deal with non thread-safeness of the ECS.
	std::mutex& _ecsMutex;

	///@brief If you want to stop the SourceManager during playing, set it to false.
	bool _isListening = true;

	///@brief Finds a SourcePlayer that is not currently used by any component.
	///@returns Index of the non-used SourcePlayer if it was found, -1 otherwise.
	int _findEmptyPlayer();

	///@brief Finds an empty SourcePlayer and assign it to the AudioSourceComponent.
	///@param source ECS component with info about the sound source.
	///@throws AudioException if a non-used SourcePlayer can't be found.
	void _assignSource(AudioSourceComponent& source);

	///@brief Asks the assigned SourcePlayer to continue playing the sound.
	/// If it's paused or hasn't been started yet, start playing.
	///@param source ECS component with info about the sound source.
	void _runSource(AudioSourceComponent& source);

	///@brief Asks the assigned SourcePlayer to stop playing the sound.
	/// The sound will be automatically removed after some time.
	///@param source ECS component with info about the sound source.
	///@returns False if the SourcePlayer couldn't stop the sound
	///(because some buffers haven't been processed yet). True otherwise.
	bool _stopSource(AudioSourceComponent& source);

	///@brief Unassigns the SourcePlayer from its AudioSourceComponent,
	/// so it can be used for other AudioSourceComponents.
	/// Marks the AudioSourceComponent as ignored until the user asks to play it.
	///@param source ECS component with info about the sound source.
	void _removeSource(AudioSourceComponent& source);

	///@brief Asks the assigned SourcePlayer to pause playing the sound.
	///@param source ECS component with info about the sound source.
	void _pauseSource(AudioSourceComponent& source);

	///@brief Asks the assigned SourcePlayer to get all the sound parameters
	/// from the AudioSourceComponent.
	///@param source ECS component with info about the sound source.
	void _updateSource(AudioSourceComponent& source);

	///@brief Sends current parameters of the Listener to OpenAL.
	void _updateListener();

public:

	///@brief Constructor. Initializes all SourcePlayers.
	///@param soundBank SoundBank object responsible for loading all sounds from the disk.
	///@param domain ECS domain used to access all currently used AudioSourceComponents.
	///@param ecsMutex Mutex used to synchronize the execution of ECS between threads.
	AudioManager(SoundBank* soundBank, ecs::Domain* domain, std::mutex& ecsMutex);

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
};
} // namespace arch::audio
