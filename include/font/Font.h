#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include "FontStyle.h"
#include <utils/OptRef.h>
#include <utils/StringViewComparator.h>
#include <utils/StringViewHasher.h>

namespace arch::font {

class FontAtlas;
class FontDB;

class Font {
public:

	static OptRef<Font> get(std::string_view familyName) noexcept;

	bool hasStyle(unsigned int style) const noexcept;

	bool hasRegular() const noexcept;
	bool hasItalic() const noexcept;
	bool hasBold() const noexcept;
	bool hasBoldItalic() const noexcept;

	std::string_view familyName() const noexcept;
	std::filesystem::path path(unsigned int style) const noexcept;

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
	std::string _paths[4];
	unsigned int _styles{};
};

} // namespace arch::font
