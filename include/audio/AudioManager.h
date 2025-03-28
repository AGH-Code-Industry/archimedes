#pragma once

#include <audio/AudioSourceComponent.h>
#include <audio/Listener.h>
#include <audio/SourcePlayer.h>
#include <ecs/Domain.h>

namespace arch::audio {

/// @brief State of SourcePlayer.
/// 'Unused' means that SourcePlayer is available for assignment.
/// When SourcePlayer is marked as 'stopped'. the manager tries to finish the sound
/// (clean all internal buffers).
/// Each SourcePlayer that cleaned its own buffers gets 'removed' state.
/// After that, the manager should free the SourcePlayer, marking it as 'unused'.
enum SourceState {
	playing, paused, stopped, removed, unused
};

/// @brief Stores all SourcePlayers on the scene and synchronizes their work.
/// Allows for automatic removal of sounds that stopped playing.
class AudioManager {
	///@brief All SourcePlayers on the scene.
	SourcePlayer _sources[16];

	///@brief States of each SourcePlayer.
	SourceState _sourceStates[16];

	///@brief Listener object used for calculating relative distance and velocity.
	/// Also controls loudness of all played sounds.
	Listener _listener;

	///@brief Each SourcePlayer has a flag telling if it should be marked as removed
	///after finishing the sound on its own (i.e. the user didn't stop the sound yourself)
	bool _dontRemoveFinished[16]{};

	///@brief Tells if the Listener was assigned to a ListenerComponent.
	bool _listenerSet = false;

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
	void _assignSource(AudioSourceComponent& source);

public:

	///@brief Constructor. Initializes all SourcePlayers.
	///@param soundBank SoundBank object responsible for loading all sounds from the disk.
	explicit AudioManager(SoundBank* soundBank);

	///@brief Destructor.
	/// Currently, it does almost nothing (apart from logging).
	~AudioManager();

	///@brief Controls the audio playback that is done by SourcePlayers.
	/// Every frame, each playing SourcePlayer will load new data to its buffers.
	/// Also, the method controls the state of the players: if a player finishes
	/// on its own, it's marked as stopped. If its buffers are cleaned, it's then marked as removed.
	///@warning This is a blocking function, and it should be used in a separate thread.
	void play();

	///@brief Stops the AudioManager.
	void stop();

	///@brief Starts playing the sound. If the source hasn't an assigned SourcePlayer yet,
	///assigns it. If the sound is already playing, rewinds it. If it's paused, continues it.
	///Updates source parameters after use.
	///@param source ECS component of the audio source.
	void playSource(AudioSourceComponent& source);

	///@brief Pauses playing the sound. Updates source parameters after use.
	///@param source ECS component of the audio source.
	///@throws AudioException if the source isn't assigned.
	void pauseSource(const AudioSourceComponent& source);

	///@brief Stops playing the sound. Updates source parameters after use.
	///Stopped source can't be played/paused until it's marked as unused.
	///@param source ECS component of the audio source.
	///@throws AudioException if the source isn't assigned.
	void stopSource(const AudioSourceComponent& source);

	///@brief Synchronizes the AudioManager's thread with ECS.
	///Updates the SourcePlayers and the Listener according to their components.
	///Marks removed SourcePlayers as unused while unassigning them from the components.
	///Checks if there is any ListenerComponent on the scene.
	///@param domain ECS domain of the scene.
	///@warning Should be called every frame in the main application loop.
	void synchronize(ecs::Domain& domain);

	///@brief Send all the sound parameters from the AudioSourceComponent
	///to the SourcePlayer.
	///@param source ECS component of the audio source.
	///@throws AudioException if the source isn't assigned.
	void updateSource(const AudioSourceComponent& source);

	///@brief Send all the listener parameters from the ListenerComponent
	///to the Listener.
	///@param listener ECS component of the listener.
	void updateListener(const ListenerComponent& listener);

	///@brief Assign Listener to a ListenerComponent. Unassign it from
	///all other components. Updates listener after use.
	///@param listener ECS component of the listener.
	///@param domain ECS domain of the scene.
	void setListener(ListenerComponent& listener, ecs::Domain& domain);

	///@brief Get state of a SourcePlayer assigned to the AudioSourceComponent.
	///Returns 'unused' if a player isn't assigned.
	///@param source ECS component of the audio source.
	SourceState getState(const AudioSourceComponent& source) const;
};
} // namespace arch::audio
