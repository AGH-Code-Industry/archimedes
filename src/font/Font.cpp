#include <ranges>

#include <font/Font.h>
#include <font/FontDB.h>
#include <freetype2/ft2build.h>
#include <utils/Assert.h>
#include FT_FREETYPE_H

#define ft (reinterpret_cast<FT_Library&>(FontDB::get()._pimpl))

namespace fs = std::filesystem;

namespace arch::font {

Font::~Font() noexcept {
	for (auto&& [path, pimpl] : std::views::values(_styles)) {
		FT_Done_Face((FT_Face)pimpl);
	}
}

OptRef<Font> Font::get(std::string_view familyName) noexcept {
	return FontDB::get().get(familyName);
}

bool Font::hasStyle(std::string_view style) const noexcept {
	return _styles.contains(style);
}

bool Font::hasRegular() const noexcept {
	return hasStyle("Regular");
}

bool Font::hasItalic() const noexcept {
	return hasStyle("Italic");
}

bool Font::hasBold() const noexcept {
	return hasStyle("Bold");
}

bool Font::hasBoldItalic() const noexcept {
	return hasStyle("Bold Italic");
}

size_t Font::styleCount() const noexcept {
	return _styles.size();
}

std::string_view Font::familyName() const noexcept {
	return _familyName;
}

std::string_view Font::path(std::string_view style) const noexcept {
	auto found = _styles.find(style);
	if (found != _styles.end()) {
		auto& [path, _] = found->second;
		return path;
	}
	return {};
}

auto Font::styles() const noexcept -> decltype(std::views::keys(*std::declval<const StylesSet*>())) {
	return std::views::keys(_styles);
}

} // namespace arch::font
