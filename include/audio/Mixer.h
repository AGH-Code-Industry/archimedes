#pragma once
#include <audio/Source.h>
#include <audio/Listener.h>

namespace arch::audio {
	///@brief Adds effects to played sounds.
	class Mixer {
		///@brief A pointer to Listener from SourceManager.
		Listener* _listener{};
	};
}