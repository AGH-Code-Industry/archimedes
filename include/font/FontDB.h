#pragma once

#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "FontStyle.h"
#include "utils/OptRef.h"
#include <utils/StringViewComparator.h>
#include <utils/StringViewHasher.h>

namespace arch::font {

class Font;

class FontDB {
	using MapType = std::unordered_map<std::string, Font, utils::StringViewHasher, utils::StringViewComparator>;

public:

	FontDB(const FontDB&) noexcept = delete;

	~FontDB() noexcept;

	static FontDB& get() noexcept;

	bool exists(std::string_view familyName) const noexcept;
	OptRef<Font> get(std::string_view familyName) noexcept;

	MapType::const_iterator begin() const noexcept;
	MapType::const_iterator end() const noexcept;

private:

	friend class Font;

	FontDB() noexcept;

	static std::unique_ptr<FontDB> _singleton;

	void _findAndAddFonts() noexcept;

	MapType _fonts;
	char* _pimpl;
};

} // namespace arch::font
