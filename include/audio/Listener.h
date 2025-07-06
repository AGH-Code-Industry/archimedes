#pragma once
#include <audio/ListenerComponent.h>
#include <physics/components/RigidBodyComponent.h>
#include <scene/components/TransformComponent.h>

namespace arch::audio {
///@brief Updates data about the listener of all played sounds.
/// Used for calculating effects of relative speed and distance,
/// also controls how loud all the sounds will be.
class Listener {
public:
	///@brief Sends the parameters to the OpenAL context.
	///@param listener ECS component with sound data.
	///@param transform ECS component with position.
	///@param moveable ECS component with velocity.
	void update(const ListenerComponent& listener, const scene::components::TransformComponent& transform,
		const physics::RigidBodyComponent& moveable);

	///@brief Sends the parameters to the OpenAL context.
	///Doesn't use spatial data.
	///@param listener ECS component with sound data.
	void update(const ListenerComponent& listener);
};
} // namespace arch::audio
