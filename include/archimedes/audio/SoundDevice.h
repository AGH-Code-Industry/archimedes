#pragma once

namespace arch::audio {
/// @brief Represents audio device and context needed for OpenAL calls
class SoundDevice {
public:
	/// @brief Finds the device and creates context
	SoundDevice();

	/// @brief Clears the context and closes the device
	~SoundDevice();

	/// @brief System's default audio device
	/// @warning If you want to use it directly, access it by std::reinterpret_cast<ALCdevice*>
	char* device;

	/// @brief Context for OpenAL calls
	/// @warning If you want to use it directly, access it by std::reinterpret_cast<ALCcontext*>
	char* context;
};

} // namespace arch::audio
