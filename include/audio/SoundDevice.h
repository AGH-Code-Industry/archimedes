#pragma once
#include <AL/alc.h>

namespace arch::audio {
/// @brief Represents audio device and context needed for OpenAL calls
class SoundDevice {
public:
	/// @brief Finds the device and creates context
	SoundDevice();

	/// @brief Clears the context and closes the device
	~SoundDevice();

	/// @brief System's default audio device
	ALCdevice* alcDevice;

	/// @brief Context for OpenAL calls
	ALCcontext* alcContext;
};

} // namespace arch::audio
