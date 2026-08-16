#pragma once

#include <source_location>

namespace arch::utils {

/// @brief Equivalent of std::source_location, but also provides short function name
class SimpleSourceLocation {
public:
	/// @brief Constructor
	constexpr SimpleSourceLocation() noexcept = default;

	/// @brief Returns source location at the call
	static consteval SimpleSourceLocation current(
		const char* functionName = __builtin_FUNCTION(),
		const std::source_location loc = std::source_location::current()
	) noexcept {
		SimpleSourceLocation result;
		result._shortFunctionName = functionName;
		result._functionName = loc.function_name();
		result._fileName = loc.file_name();
		result._line = loc.line();
		result._col = loc.column();

		return result;
	}

	/// @brief Returns function name
	constexpr const char* shortFunctionName() const noexcept { return _shortFunctionName; }

	/// @brief Returns long function name
	constexpr const char* functionName() const noexcept { return _functionName; }

	/// @brief Returns source file name
	constexpr const char* fileName() const noexcept { return _fileName; }

	/// @brief Returns source line
	constexpr uint32_t line() const noexcept { return _line; }

	/// @brief Returns source column
	constexpr uint32_t column() const noexcept { return _col; }

private:
	const char* _functionName = "";
	const char* _shortFunctionName = "";
	const char* _fileName = "";
	uint32_t _line = {};
	uint32_t _col = {};
};

} // namespace arch::utils
