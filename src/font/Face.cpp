#include <array>
#include <execution>
#include <filesystem>
#include <iostream>

#include <font/Face.h>
#include <font/MSDFAtlasGen.h>
#include <freetype2/ft2build.h>
#include <json/json.h>
#include FT_FREETYPE_H

#include <stb_image.h>

namespace fs = std::filesystem;

namespace arch::font {

Face::~Face() noexcept {
	FT_Done_Face(reinterpret_cast<FT_Face>(_pimpl));
}

std::string_view Face::fontName() const noexcept {
	return _familyName;
}

std::string_view Face::styleName() const noexcept {
	return std::string_view(reinterpret_cast<FT_Face>(_pimpl)->style_name);
}

std::string_view Face::fontPath() const noexcept {
	return _fontPath;
}

const Face& Face::assure() noexcept {
	if (!generated()) {
		Logger::debug("{}__{}", _familyName, styleName());
		_generate();
	}
	if (!loaded()) {
		load();
	}
	return *this;
}

bool Face::load() noexcept {
	if (loaded()) {
		return false;
	}

	std::string fileName = "msdf_cache/";
	fileName.append(_familyName).append("__").append(styleName());
	std::ranges::replace(fileName, ' ', '_');

	fs::create_directory("msdf_cache");

	auto jsonFile = std::ifstream(fileName + ".json");
	if (!jsonFile.good() || !fs::is_regular_file(fileName + ".png")) {
		return false;
	}

	std::string jsonInput;

	Json::Value json;
	jsonFile >> json;

	auto&& atlas = json["atlas"];
	_atlas.width = atlas["width"].asUInt();
	_atlas.height = atlas["height"].asUInt();
	_atlas.resolution = atlas["size"].asUInt();

	auto&& metrics = json["metrics"];
	_metrics.lineHeight = metrics["lineHeight"].asFloat();
	_metrics.ascender = metrics["ascender"].asFloat();
	_metrics.descender = metrics["descender"].asFloat();
	_metrics.underlineY = metrics["underlineY"].asFloat();
	_metrics.underlineThickness = metrics["underlineThickness"].asFloat();

	for (auto&& glyph : json["glyphs"]) {
		auto&& glyphData = _glyphs[glyph["unicode"].asUInt()];

		glyphData.advance = glyph["advance"].asFloat();

		auto planeBoundsPtr = glyph.find("planeBounds");
		if (planeBoundsPtr) {
			auto&& planeBounds = *planeBoundsPtr;
			glyphData.planeBounds.left = planeBounds["left"].asFloat();
			glyphData.planeBounds.top = planeBounds["top"].asFloat();
			glyphData.planeBounds.right = planeBounds["right"].asFloat();
			glyphData.planeBounds.bottom = planeBounds["bottom"].asFloat();

			auto&& atlasBounds = glyph["atlasBounds"];
			glyphData.atlasBounds.left = atlasBounds["left"].asFloat();
			glyphData.atlasBounds.top = atlasBounds["top"].asFloat();
			glyphData.atlasBounds.right = atlasBounds["right"].asFloat();
			glyphData.atlasBounds.bottom = atlasBounds["bottom"].asFloat();

			glyphData.width = glyphData.atlasBounds.right - glyphData.atlasBounds.left;
			glyphData.height = glyphData.atlasBounds.bottom - glyphData.atlasBounds.top;

			glyphData.uv.first = { glyphData.atlasBounds.left / _atlas.width,
								   glyphData.atlasBounds.top / _atlas.height };
			glyphData.uv.second = { glyphData.atlasBounds.right / _atlas.width,
									glyphData.atlasBounds.bottom / _atlas.height };
		}
	}

	for (auto&& kerning : json["kerning"]) {
		auto [it, ignored] = _kerning.insert({
			{ kerning["unicode1"].asUInt(), kerning["unicode2"].asUInt() },
			kerning["advance"].asFloat()
		 });
	}

	auto textureData = std::vector<Color>(atlasWidth() * atlasHeight());
	stbi_set_flip_vertically_on_load(true);
	int ignored; // width & height loaded from .json
	unsigned char* loadedTextureData =
		stbi_load((fileName + ".png").c_str(), &ignored, &ignored, &ignored, STBI_rgb_alpha);

	std::for_each(
		std::execution::par_unseq,
		textureData.begin(),
		textureData.end(),
		[&textureData, &loadedTextureData](Color& color) {
			const auto i = &color - textureData.data();
			color.r = loadedTextureData[4 * i + 0] / 255.f;
			color.g = loadedTextureData[4 * i + 1] / 255.f;
			color.b = loadedTextureData[4 * i + 2] / 255.f;
			color.a = loadedTextureData[4 * i + 3] / 255.f;
		}
	);

	auto&& renderer = *gfx::Renderer::getCurrent();

	_atlasTexture = renderer.getTextureManager()->createTexture2D(atlasWidth(), atlasHeight(), textureData.data());

	stbi_image_free(loadedTextureData);

	_placeholder = _findPlaceholder();

	return true;
}

void Face::_generate() noexcept {
	if (generated()) {
		return;
	}

	fs::create_directory("msdf_cache");

	std::string command;

	std::string fileName;
	fileName.append(_familyName).append("__").append(styleName());
	std::ranges::replace(fileName, ' ', '_');

	std::string null = ARCHIMEDES_WINDOWS ? "nul" : "/dev/null";

	// redirect output to null
	command = std::format(
		"\"\"{0}\" -type msdf -font \"{1}\" -format png -imageout {2}.png -json {2}.json -size {4} "
		"-pxrange "
		"4 -yorigin top -chars \"[0x0, 0x100000]\" > {3} 2> {3}\"",
		MSDFAtlasGen::get().path(),
		_fontPath,
		fileName,
		null,
		MSDFAtlasGen::glyphResolution
	);
	std::system(command.c_str());

	for (auto&& ext : { ".png", ".json" }) {
		fs::rename(fileName + ext, "msdf_cache/" + fileName + ext);
	}
}

bool Face::generated() const noexcept {
	std::string fileName = "msdf_cache/";
	fileName.append(_familyName).append("__").append(styleName());
	std::ranges::replace(fileName, ' ', '_');
	return fs::is_regular_file(fileName + ".json") && fs::is_regular_file(fileName + ".png");
}

bool Face::loaded() const noexcept {
	return _atlas.width != 0 && _atlas.height != 0;
}

unsigned int Face::atlasWidth() const noexcept {
	return _atlas.width;
}

unsigned int Face::atlasHeight() const noexcept {
	return _atlas.height;
}

size_t Face::glyphCount() const noexcept {
	return _glyphs.size();
}

size_t Face::glyphCountGen() noexcept {
	return assure()._glyphs.size();
}

OptRef<const GlyphData> Face::glyphData(char32_t unicode) const noexcept {
	auto found = _glyphs.find(unicode);
	if (found != _glyphs.end()) {
		return found->second;
	}
	return std::nullopt;
}

OptRef<const GlyphData> Face::glyphDataGen(char32_t unicode) noexcept {
	return assure().glyphData(unicode);
}

OptRef<const GlyphData> Face::operator[](char32_t unicode) noexcept {
	return glyphDataGen(unicode);
}

OptRef<const GlyphData> Face::operator[](char32_t unicode) const noexcept {
	return glyphData(unicode);
}

float Face::kerning(char32_t right, char32_t left) const noexcept {
	if (!loaded()) {
		return 0;
	}
	auto found = _kerning.find({ right, left });
	return found != _kerning.end() ? found->second : 0;
}

const Face::KerningMap& Face::kerningMap() const noexcept {
	return _kerning;
}

const Face::Metrics& Face::metrics() const noexcept {
	return _metrics;
}

unsigned int Face::resolution() const noexcept {
	return _atlas.resolution;
}

Ref<gfx::texture::Texture> Face::atlasTexture() const noexcept {
	if (!loaded()) {
		return nullptr;
	}
	return _atlasTexture;
}

Ref<gfx::texture::Texture> Face::atlasTextureGen() noexcept {
	return assure().atlasTexture();
}

const GlyphData* Face::_findPlaceholder() const noexcept {
	constexpr auto potentialPlaceholders =
		std::array{ U'\xfffd', U'\xfffc', U'\x25a1', U'\x25af', U'\x2b1a', U'?', U' ' };

	for (auto placeholder : potentialPlaceholders) {
		auto glyphDataOpt = glyphData(placeholder);
		if (glyphDataOpt) {
			return &*glyphDataOpt;
		}
	}
	Logger::error("Style '{}' of '{}' does not contain '?' or ' '!", styleName(), _familyName);
	return nullptr; // impossible for font to not have
					// '?' or ' '
}

const GlyphData& Face::placeholder() const noexcept {
	return *_placeholder;
}

} // namespace arch::font
