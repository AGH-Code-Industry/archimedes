#pragma once

#include <audio/Listener.h>
#include <audio/Source.h>
#include <audio/SourceComponent.h>
#include <ecs/Domain.h>

namespace arch::audio {

	/// @brief Stores all Sources on the scene and synchronizes their work.
	class SourceManager {

		///@brief ECS Domain object allowing to access all SourceComponents
		ecs::Domain *_domain;

		///@brief All Sources on the scene.
		Source _sources[16];

		bool _sourceUsed[16] = {false};

		///@brief Listener object used for calculating relative distance and velocity.
		///Also controls loudness of all played sounds.
		Listener _listener;

		///@brief Sound bank storing all clips.
		SoundBank* _soundBank;

		std::mutex& _mutex;

		///@brief Find a Source that is not currently used by any component.
		int _findEmptySource();

		void _playSource(SourceComponent& component);
		void _assignSource(SourceComponent& component);
		bool _stopSource(SourceComponent& component);
		void _removeSource(SourceComponent& component);
		void _pauseSource(SourceComponent& component);
		void _updateSource(SourceComponent& component);


		///@brief Send current parameters of the Listener to OpenAL.
		void _updateListener();

		public:

		///@brief If you want to stop the SourceManager during playing, set it to false.
		//TODO: there should be a better way (a new method maybe?)
		bool isListening = true;

		SourceManager(SoundBank* soundBank, ecs::Domain *domain, std::mutex& mutex);

		///@brief Destructor.
		///Currently, it does almost nothing (apart from logging).
		~SourceManager();

		///@brief Play all the Sources.
		///Every frame, each playing Source will be updated.
		///@warning This is a blocking function, and it should be used in a separate thread.
		void play();

		void stop();

	};
}