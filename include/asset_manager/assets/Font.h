#pragma once

#include <vector>

namespace arch::assetManager::assets {

#pragma pack(push, 1)

struct FontAssetHeader {
	uint32_t magic;
	uint32_t version;

	uint32_t metricsOffset;
	uint32_t atlasMetaOffset;
	uint32_t glyphsOffset;
	uint32_t kerningOffset;
	uint32_t pixelsOffset;

	uint32_t glyphCount;
	uint32_t kerningPairCount;
};

struct FontMetrics {
	float lineHeight;
	float ascender;
	float descender;
};

struct AtlasMetadata {
	uint32_t width;
	uint32_t height;
	uint32_t format;
};

struct GlyphData {
	uint32_t unicode;
	float advance;

	float planeLeft, planeBottom, planeRight, planeTop;

	float atlasLeft, atlasBottom, atlasRight, atlasTop;
};

struct KerningPair {
	uint32_t unicode1;
	uint32_t unicode2;
	float advanceCorrection;
};

#pragma pack(pop)

struct AtlasImageData {
	uint32_t width;
	uint32_t height;
	uint32_t format;
	std::vector<uint8_t> pixels;
};

struct Font {
	FontMetrics metrics;
	AtlasImageData atlas;
	std::vector<GlyphData> glyphs;
	std::vector<KerningPair> kerningPairs;
};

} // namespace arch::assetManager::assets
