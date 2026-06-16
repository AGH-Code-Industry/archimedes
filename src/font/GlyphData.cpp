#include <archimedes/font/GlyphData.h>

bool arch::font::GlyphData::advanceOnly() const noexcept {
	return std::isnan(width);
}
