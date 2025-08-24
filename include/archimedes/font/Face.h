#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "GlyphData.h"
#include <archimedes/Mmath.h>
#include <archimedes/gfx/Renderer.h>
#include <archimedes/utils/OptRef.h>
#include <archimedes/utils/PairHasher.h>

namespace arch::font {

/// @brief Represents font face
class Face {
	// std::pair is not hashable by default :(
	using KerningMap = std::unordered_map<std::pair<char32_t, char32_t>, float, utils::PairHasher<char32_t>>;

public:

	/// @brief Struct holding various metrics of face
	struct Metrics {
		/// @brief Height of line (em)
		float lineHeight;
		/// @brief Size of tallest glyph part above the base (em)
		float ascender;
		/// @brief Size of tallest glyph part below the base (em)
		float descender;
		/// @brief Position of underline below the base (em)
		float underlineY;
		/// @brief Thickness of underline (em)
		float underlineThickness;
	};

	/// @brief Destructor. Releases face handle
	~Face() noexcept;

	/// @brief Returns family name of font
	std::string_view fontName() const noexcept;
	/// @brief Returns style name of face
	std::string_view styleName() const noexcept;
	/// @brief Returns absolute path to file containing this style
	std::string_view fontPath() const noexcept;

	/// @brief Generates atlas + metadata and loads them
	/// @return This Face object
	const Face& assure() noexcept;
	/// @brief Loads face atlas and metadata
	/// @return true if succesfully loaded everything, false otherwise
	bool load() noexcept;
	/// @brief Checks if atlas for this face was generated
	bool generated() const noexcept;
	/// @brief Checks if atlas for this face was loaded
	bool loaded() const noexcept;

	/// @brief Returns atlas width in pixels
	unsigned int atlasWidth() const noexcept;
	/// @brief Returns atlas height in pixels
	unsigned int atlasHeight() const noexcept;

	/// @brief Returns glyph count in atlas
	/// @return 0 if loaded, != 0 otherwise
	size_t glyphCount() const noexcept;
	/// @brief Assures atlas and returns glyph count in atlas
	size_t glyphCountGen() noexcept;

	/// @brief Returns optional with GlyphData of given unicode character
	/// @param unicode - character to obtain
	/// @return Non-empty optional when found, empty if not
	OptRef<const GlyphData> glyphData(char32_t unicode) const noexcept;
	/// @brief Assures atlas and returns optional with GlyphData of given unicode character
	/// @param unicode - character to obtain
	/// @return Non-empty optional when found, empty if not
	OptRef<const GlyphData> glyphDataGen(char32_t unicode) noexcept;
	/// @brief Assures atlas and returns optional with GlyphData of given unicode character
	/// @param unicode - character to obtain
	/// @return Non-empty optional when found, empty if not
	OptRef<const GlyphData> operator[](char32_t unicode) noexcept;
	/// @brief Returns optional with GlyphData of given unicode character
	/// @param unicode - character to obtain
	/// @return Non-empty optional when found, empty if not
	OptRef<const GlyphData> operator[](char32_t unicode) const noexcept;
	/// @brief Returns GlyphData of placeholder character to be used when some glyph does not exist
	const GlyphData& placeholder() const noexcept;

	/// @brief Returns kerning vaule between given unicode characters
	/// @param right - right character
	/// @param left - left character
	/// @return != 0 if kerning is defined, 0 if not
	float kerning(char32_t right, char32_t left) const noexcept;
	/// @brief Returns readonly kerning map, with char32_t pairs as keys
	const KerningMap& kerningMap() const noexcept;
	/// @brief Returns face metrics
	const Metrics& metrics() const noexcept;
	/// @brief Returns atlas resolution (pixels per em)
	unsigned int resolution() const noexcept;

	/// @brief Returns Ref to loaded atlas texture
	/// @return Ref to atlas texture, empty Ref if not loaded
	Ref<gfx::texture::Texture> atlasTexture() const noexcept;
	/// @brief Assures atlas and returns Ref to loaded atlas texture
	/// @return Ref to atlas texture, empty Ref if not loaded
	Ref<gfx::texture::Texture> atlasTextureGen() noexcept;

private:

	Face() noexcept = default;

	friend class FontDB;
	friend class MSDFAtlasGen;

	// generates atlas + metadata and saves it in cache
	void _generate() noexcept;
	// finds placeholder data (commonly used placeholders are not guaranteed to exist in a face)
	const GlyphData* _findPlaceholder() const noexcept;

	std::string_view _familyName;
	std::string _fontPath;
	char* _pimpl{};

	struct {
		unsigned int width;
		unsigned int height;
		unsigned int resolution;
	} _atlas;

	Metrics _metrics;

	std::unordered_map<char32_t, GlyphData> _glyphs{};
	KerningMap _kerning;
	Ref<gfx::texture::Texture> _atlasTexture;
	const GlyphData* _placeholder;
};

} // namespace arch::font
