#pragma once

#include <cmath>

namespace arch::font {

struct GlyphData {
	float advance = std::nan("");

	struct {
		float left = std::nan("");
		float top = std::nan("");
		float right = std::nan("");
		float bottom = std::nan("");
	} planeBounds, atlasBounds;

	struct {
		struct {
			float x = std::nan("");
			float y = std::nan("");
		} first, second;
	} uv;

	float width = std::nan("");
	float height = std::nan("");

	bool advanceOnly() const noexcept;
};

} // namespace arch::font
