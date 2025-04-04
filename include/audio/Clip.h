#pragma once

#include <string>
#include <vector>


namespace arch::audio {

/// @brief Tells the format of sound file.
enum ClipFormat {
	mono,
	stereo
};

/// @brief Allows for loading audio files and streaming their data to SourcePlayers.
class Clip {
	/// @brief Sound's sample rate.
	int _sampleRate{};

	/// @brief Size of all samples in bytes.
	int _dataSize{};

	/// @brief Sound's number of channels.
	unsigned short _channelsNumber{};

	/// @brief Sound's all samples.
	std::vector<short> _soundData;

	/// @brief Path to the sound file.
	std::string _filePath;

	/// @brief Number of items per buffer.
	/// An item is a block of samples, one for each channel.
	const std::size_t _sampleItems = 4096;

	/// @brief Tells if the Clip's data is loaded.
	bool _isLoaded = false;

public:
	/// @brief Constructor.
	/// @param path Path to the sound file.
	explicit Clip(const std::string& path): _filePath(path) {}

	/// @brief Loads the sound file.
	/// @throws AudioException If it couldn't open the file.
	void load();

	/// @brief Unloads the sound file.
	void unload();

	/// @brief Destructor.
	~Clip();

	/// @brief Fill a buffer with samples from sound file.
	/// @param buffer Buffer to be filled.
	/// @param cursor Current position in the sound file.
	/// Each object which uses the Clip should know its own position.
	/// @param isLooped If the sound has to be looped, set it to true.
	/// @throws AudioException If clip is not loaded.
	/// @returns True if the end of sound was reached, false otherwise.
	bool fillBuffer(std::vector<short>& buffer, std::size_t& cursor, bool isLooped) const;

	/// @brief Get number of all elements in a buffer.
	/// The number depends on the number of channels, so it needs to be calculated by the Clip.
	/// @returns Number of buffer's element.
	std::size_t getBufferElements() const;

	/// @brief Get the sound's OpenAL format.
	/// @returns OpenAL format value.
	ClipFormat getFormat() const;

	/// @brief Getter of the _sampleRate parameter.
	/// @see _sampleRate
	/// @returns Sound's sample rate.
	int getSampleRate() const;

	/// @brief Getter of the _isLoaded property.
	/// @see _isLoaded
	/// @return True if clip is now loaded, false otherwise.
	bool getIsLoaded() const;
};
} // namespace arch::audio
