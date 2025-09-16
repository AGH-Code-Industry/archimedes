#pragma once

#include <cstdlib>
#include <fstream>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace arch::font {

class Font;

/// @brief Helper class for obtaining msdf-atlas-gen's path
class MSDFAtlasGen {
public:

	/// @brief Deleted copy constructor
	MSDFAtlasGen(const MSDFAtlasGen&) noexcept = delete;

	/// @brief Returns singleton object
	static MSDFAtlasGen& get() noexcept;

	/// @brief Returns absolute path to msdf-atlas-gen
	std::string_view path() const noexcept;

	/// @brief Resolution scale of glyphs passed to msdf-atlas-gen
	static constexpr unsigned int glyphResolution = 32;

private:

	// reads cached path from cache file
	// or invokes _cachePath() if cache is bad
	MSDFAtlasGen() noexcept;

	static std::unique_ptr<MSDFAtlasGen> _singleton;

	// obtains msdf-atlas-gen's path from conan
	void _cachePath() noexcept;

	std::string _binaryPath;
};

} // namespace arch::font
