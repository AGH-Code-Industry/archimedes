#pragma once

#include <filesystem>
#include <ranges>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>

#include "FontStyle.h"
#include <utils/OptRef.h>
#include <utils/StringViewComparator.h>
#include <utils/StringViewHasher.h>

namespace arch::font {

class MSDFAtlasGen;
class FontDB;
class Face;

class Font {
	using StylesSet = std::unordered_map<std::string_view, Face, utils::StringViewHasher, utils::StringViewComparator>;

public:

	static OptRef<Font> get(std::string_view name) noexcept;

	bool hasStyle(std::string_view style) const noexcept;

	bool hasRegular() const noexcept;
	bool hasItalic() const noexcept;
	bool hasBold() const noexcept;
	bool hasBoldItalic() const noexcept;

	size_t styleCount() const noexcept;

	std::string_view name() const noexcept;
	OptRef<Face> operator[](std::string_view style) noexcept;

	OptRef<Face> face() noexcept;
	OptRef<Face> face(std::string_view style) noexcept;
	OptRef<Face> regular() noexcept;
	OptRef<Face> bold() noexcept;
	OptRef<Face> italic() noexcept;
	OptRef<Face> boldItalic() noexcept;

	auto styles() const noexcept -> decltype(std::views::keys(*std::declval<const StylesSet*>()));

	// might take some time
	// bool assure(FontStyle style) const noexcept;

private:

	friend class FontDB;

	friend class std::unordered_map<std::string, Font, utils::StringViewHasher, utils::StringViewComparator>;

	Font() noexcept = default;

	std::string_view _familyName{};
	StylesSet _styles{};
};

} // namespace arch::font
