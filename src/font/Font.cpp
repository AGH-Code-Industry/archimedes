#include <font/Font.h>
#include <font/FontDB.h>
#include <freetype2/ft2build.h>
#include <utils/Assert.h>
#include FT_FREETYPE_H

#define ft (reinterpret_cast<FT_Library&>(FontDB::get()._pimpl))
#define faces (reinterpret_cast<Impl*&>(_pimpl))

namespace fs = std::filesystem;

struct Impl {
	FT_Face face[3];
};

namespace arch::font {

OptRef<Font> Font::get(std::string_view familyName) noexcept {
	return FontDB::get().get(familyName);
}

bool Font::hasStyle(unsigned int style) const noexcept {
	return _styles & style;
}

bool Font::hasRegular() const noexcept {
	return hasStyle(FontStyle::regular);
}

bool Font::hasItalic() const noexcept {
	return hasStyle(FontStyle::italic);
}

bool Font::hasBold() const noexcept {
	return hasStyle(FontStyle::bold);
}

bool Font::hasBoldItalic() const noexcept {
	return hasStyle(FontStyle::italic | FontStyle::bold);
}

std::string_view Font::familyName() const noexcept {
	return _familyName;
}

std::filesystem::path Font::path(unsigned int style) const noexcept {
	return _paths[(style >> 1) & 0b11];
}

} // namespace arch::font
