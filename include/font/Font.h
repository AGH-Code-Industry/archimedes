#pragma once

#include <filesystem>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_map>

#include "FontStyle.h"
#include <utils/OptRef.h>
#include <utils/StringViewComparator.h>
#include <utils/StringViewHasher.h>

namespace arch::font {

class FontAtlas;
class FontDB;

class Font {
	using StylesSet = std::unordered_map<
		std::string_view,
		std::tuple<std::string, char*>,
		utils::StringViewHasher,
		utils::StringViewComparator>;

public:

	~Font() noexcept;

	static OptRef<Font> get(std::string_view familyName) noexcept;

	bool hasStyle(std::string_view style) const noexcept;

	bool hasRegular() const noexcept;
	bool hasItalic() const noexcept;
	bool hasBold() const noexcept;
	bool hasBoldItalic() const noexcept;

	size_t styleCount() const noexcept;

	std::string_view familyName() const noexcept;
	std::string_view path(std::string_view style) const noexcept;

	auto styles() const noexcept -> decltype(std::views::keys(*std::declval<const StylesSet*>()));

	// might take some time
	// bool assure(FontStyle style) const noexcept;

private:

	friend class FontDB;
	friend void findAndAddFontsWindows(
		std::unordered_map<std::string, Font, utils::StringViewHasher, utils::StringViewComparator>& fonts,
		char* pimpl
	) noexcept;
	friend void findAndAddFontsLinux(
		std::unordered_map<std::string, Font, utils::StringViewHasher, utils::StringViewComparator>& fonts,
		char* pimpl
	) noexcept;

	Font() noexcept = default;

	std::string_view _familyName;
	StylesSet _styles;
};

} // namespace arch::font
