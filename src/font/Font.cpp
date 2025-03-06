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

bool Font::hasStyle(std::string_view style) const noexcept {
	return _styles.contains(style);
}

bool Font::hasRegular() const noexcept {
	return hasStyle(font::regular);
}

bool Font::hasItalic() const noexcept {
	return hasStyle(font::italic);
}

bool Font::hasBold() const noexcept {
	return hasStyle(font::bold);
}

bool Font::hasBoldItalic() const noexcept {
	return hasStyle(font::boldItalic);
}

size_t Font::styleCount() const noexcept {
	return _styles.size();
}

std::string_view Font::name() const noexcept {
	return _familyName;
}

OptRef<Face> Font::face(std::string_view style) noexcept {
	if (style.empty()) {
		// return Regular as the first
		auto reg = regular();
		if (reg) {
			return reg;
		} else {
			// if no Regular, then return first found style
			auto first = _styles.begin();
			if (first != _styles.end()) {
				return first->second;
			} else {
				return std::nullopt;
			}
		}
	}

	// style was specified
	auto found = _styles.find(style);
	if (found != _styles.end()) {
		return found->second;
	}
	return std::nullopt;
}

OptRef<Face> Font::regular() noexcept {
	return face(font::regular);
}

OptRef<Face> Font::bold() noexcept {
	return face(font::bold);
}

OptRef<Face> Font::italic() noexcept {
	return face(font::italic);
}

OptRef<Face> Font::boldItalic() noexcept {
	return face(font::boldItalic);
}

OptRef<Face> Font::operator[](std::string_view style) noexcept {
	return face(style);
}

auto Font::styles() const noexcept -> decltype(std::views::keys(*std::declval<const StylesSet*>())) {
	// styles are mapped by their names
	return std::views::keys(_styles);
}

} // namespace arch::font
