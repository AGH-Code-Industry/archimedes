#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "GlyphData.h"
#include <Mmath.h>
#include <gfx/Renderer.h>
#include <utils/OptRef.h>
#include <utils/PairHasher.h>

namespace arch::font {

class Face {
	using KerningMap = std::unordered_map<std::pair<char32_t, char32_t>, float, utils::PairHasher<char32_t>>;

public:

	struct Metrics {
		float lineHeight;
		float ascender;
		float descender;
		float underlineY;
		float underlineThickness;
	};

	~Face() noexcept;

	std::string_view fontName() const noexcept;
	std::string_view styleName() const noexcept;
	std::string_view fontPath() const noexcept;

	const Face& assure() noexcept;
	bool load() noexcept;
	bool generated() const noexcept;
	bool loaded() const noexcept;

	unsigned int atlasWidth() const noexcept;
	unsigned int atlasHeight() const noexcept;

	size_t glyphCount() const noexcept;
	size_t glyphCountGen() noexcept;

	OptRef<const GlyphData> glyphData(char32_t unicode) const noexcept;
	OptRef<const GlyphData> glyphDataGen(char32_t unicode) noexcept;
	OptRef<const GlyphData> operator[](char32_t unicode) noexcept;
	OptRef<const GlyphData> operator[](char32_t unicode) const noexcept;

	float kerning(char32_t right, char32_t left) const noexcept;
	const KerningMap& kerningMap() const noexcept;
	const Metrics& metrics() const noexcept;
	unsigned int size() const noexcept;

	Ref<gfx::texture::Texture> atlasTexture() const noexcept;
	Ref<gfx::texture::Texture> atlasTextureGen() noexcept;

private:

	Face() noexcept = default;

	friend class FontDB;
	friend class MSDFAtlasGen;

	void _generate() noexcept;

	std::string_view _familyName;
	std::string _fontPath;
	char* _pimpl{};

	struct {
		unsigned int width;
		unsigned int height;
		unsigned int size;
	} _atlas;

	Metrics _metrics;

	std::unordered_map<char32_t, GlyphData> _glyphs{};
	KerningMap _kerning;
	Ref<gfx::texture::Texture> _atlasTexture;
};

} // namespace arch::font
