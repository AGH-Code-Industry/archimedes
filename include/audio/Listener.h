#pragma once
#include <audio/ListenerComponent.h>

namespace arch::audio {
///@brief Stores data about the listener of all played sounds.
/// Used for calculating effects of relative speed and distance,
/// also controls how loud all the sounds will be.
class Listener {
public:
	///@brief Sends the parameters to the OpenAL context.
	///@warning Should be called each audio frame.
	void update(const ListenerComponent& listener);
};
} // namespace arch::audio
