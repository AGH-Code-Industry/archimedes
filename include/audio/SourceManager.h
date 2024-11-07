#pragma once
#include <mutex>
#include <vector>

#include <audio/Listener.h>
#include <audio/Mixer.h>
#include <audio/SoundBank.h>
#include <audio/Source.h>
#include <audio/SourceComponent.h>
#include <ecs/Domain.h>
#include <ecs/Entity.h>

namespace arch::audio {

	/// @brief Stores all Sources on the scene and synchronizes their work.
	class SourceManager {

		ecs::Domain<ecs::e64> *_domain;

		///@brief All Sources on the scene.
		Source _sources[16];

		///@brief Listener object used for calculating relative distance and velocity.
		///Also controls loudness of all played sounds.
		Listener _listener;

		///@brief Sound bank storing all clips.
		SoundBank* _soundBank;

		int _findEmptySource();

		void _playSource(SourceComponent& component);

		void _updateSource(SourceComponent& component);


		///@brief Send current parameters of the Listener to OpenAL.
		void _updateListener();

		public:

		Mixer mixer;

		///@brief If you want to stop the SourceManager during playing, set it to false.
		//TODO: there should be a better way (a new method maybe?)
		bool isListening = true;

		///@brief Constructor.
		///@param soundBank Sound bank storing all the clips.
		SourceManager(SoundBank* soundBank, ecs::Domain<ecs::e64> *domain);

		///@brief Destructor.
		///Currently, it does almost nothing (apart from logging).
		~SourceManager();

		///@brief Play all the Sources.
		///Every frame, each playing Source will be updated.
		///Sources that stopped playing will be removed from the SourceManager.
		///@warning This is a blocking function, and it should be used in a separate thread.
		void play();

		void addSource(SourceComponent& component);

		void removeSource(SourceComponent& component);

		void pauseSource(SourceComponent& component);

		void continueSource(SourceComponent& component);
	};
}