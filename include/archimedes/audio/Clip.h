#pragma once

#include <string>
#include <vector>
#include <string_view>

namespace arch::audio {


/// @brief Allows for loading audio files and streaming their data to SourcePlayers.
class Clip {
	/// @brief Sound's sample rate.
	i32 _sampleRate{};

	/// @brief Sound's number of channels.
	u16 _channelsNumber{};

	/// @brief Path to the sound file.
	std::string _filePath;

	/// @brief Tells if the Clip's data is loaded.
	bool _isLoaded = false;

	u32 _alBuffer{};

	/// @brief Get the sound's OpenAL format.
	/// @returns OpenAL format value.
	ALenum _getFormat() const;

public:
	/// @brief Constructor.
	/// @param path Path to the sound file.
	explicit Clip(std::string_view path);

	/// @brief Loads the sound file.
	/// @throws AudioException If it couldn't open the file.
	void load();

	/// @brief Unloads the sound file.
	void unload();

	/// @brief Destructor.
	~Clip();

	/// @brief Getter of the _sampleRate parameter.
	/// @see _sampleRate
	/// @returns Sound's sample rate.
	i32 getSampleRate() const;

	/// @brief Getter of the _isLoaded property.
	/// @see _isLoaded
	/// @return True if clip is now loaded, false otherwise.
	bool getIsLoaded() const;
};
} // namespace arch::audio
