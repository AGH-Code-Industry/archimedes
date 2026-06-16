#pragma once

#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include <archimedes/utils/OptRef.h>
#include <archimedes/utils/StringViewComparator.h>
#include <archimedes/utils/StringViewHasher.h>

namespace arch {
class Engine;
}

namespace arch::font {

class Font;

/// @brief Fonts database, reads all fonts installed on system
class FontDB {
	// unordered_map, but allows for using string_views as keys
	using MapType = std::unordered_map<std::string, Font, utils::StringViewHasher, utils::StringViewComparator>;

public:

	/// @brief Deleted copy constructor
	FontDB(const FontDB&) noexcept = delete;

	/// @brief Destructor, frees all handles to fonts
	~FontDB() noexcept;

	/// @brief Returns singleton
	static FontDB& get() noexcept;

	/// @brief Checks if font of given name exists in database
	bool exists(std::string_view name) const noexcept;
	/// @brief Optionally returns font
	/// @param name - name of font to get
	OptRef<Font> get(std::string_view name) noexcept;
	/// @brief Optionally returns font
	/// @param name - name of font to get
	OptRef<Font> operator[](std::string_view name) noexcept;

	/// @brief Returns readonly iterator to the first element of font map
	MapType::const_iterator begin() const noexcept;
	/// @brief Returns readonly iterator to the past-the-last element of font map
	MapType::const_iterator end() const noexcept;

private:

	friend class Font;
	friend class ::arch::Engine;

	FontDB() noexcept;

	static std::unique_ptr<FontDB> _singleton;

	void _findAndAddFonts() noexcept;
#if ARCHIMEDES_WINDOWS
	// reads fonts from registry
	void _findAndAddFontsWindows() noexcept;
#elif ARCHIMEDES_LINUX
	// finds fonts in common locations
	void _findAndAddFontsLinux() noexcept;
#endif

	// adds font to database
	void _addFont(std::string path) noexcept;

	MapType _fonts;
	char* _pimpl;
};

} // namespace arch::font
