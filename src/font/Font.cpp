#include <ranges>

#include <font/Face.h>
#include <font/Font.h>
#include <font/FontDB.h>
#include <freetype2/ft2build.h>
#include <utils/Assert.h>
#include FT_FREETYPE_H

#define ft (reinterpret_cast<FT_Library&>(FontDB::get()._pimpl))

namespace fs = std::filesystem;

namespace arch::font {

OptRef<Font> Font::get(std::string_view name) noexcept {
	return FontDB::get().get(name);
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

std::string_view Font::name() const noexcept {
	return _familyName;
}

OptRef<Face> Font::face(std::string_view style) noexcept {
	auto found = _styles.find(style);
	if (found != _styles.end()) {
		return found->second;
	}
	return std::nullopt;
}

OptRef<Face> Font::face() noexcept {
	return regular();
}

OptRef<Face> Font::regular() noexcept {
	return face("Regular");
}

OptRef<Face> Font::bold() noexcept {
	return face("Bold");
}

OptRef<Face> Font::italic() noexcept {
	return face("Italic");
}

OptRef<Face> Font::boldItalic() noexcept {
	return face("Bold Italic");
}

OptRef<Face> Font::operator[](std::string_view style) noexcept {
	return face(style);
}

auto Font::styles() const noexcept -> decltype(std::views::keys(*std::declval<const StylesSet*>())) {
	return std::views::keys(_styles);
}

} // namespace arch::font
