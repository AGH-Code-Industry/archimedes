#pragma once

#include <cstdlib>
#include <fstream>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace arch::font {

class Font;

class MSDFAtlasGen {
public:

	MSDFAtlasGen(const MSDFAtlasGen&) noexcept = delete;

	static MSDFAtlasGen& get() noexcept;

	std::string_view path() const noexcept;

	static constexpr unsigned int glyphResolution = 32;

private:

	MSDFAtlasGen() noexcept;

	static std::unique_ptr<MSDFAtlasGen> _singleton;

	void _cachePath() noexcept;

	std::string _binaryPath;
};

} // namespace arch::font
