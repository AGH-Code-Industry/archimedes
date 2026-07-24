#pragma once

#include <exception>
#include <stacktrace>

#include <archimedes/Logger.h>

namespace arch {

/// @brief Exception class
/// @details Meant to be used only as a base class.
class Exception: public std::exception {
public:
	/// @brief Inherited from std::exception
	/// @return message()
	const char* what() const noexcept override final;

	/// @brief Returns exception title
	std::string_view title() const noexcept;

	/// @brief Returns exception message
	std::string_view message() const noexcept;

	/// @brief Returns exception stacktrace
	const std::stacktrace& stacktrace() const noexcept;

protected:
	/// @brief Base class constructor
	/// @param title - exception title
	/// @param message - exception message
	/// @param stacktrace - exception stacktrace (use std::stacktrace::current() for default value)
	Exception(std::string_view title, std::string_view message, const std::stacktrace& stacktrace);

	std::string _title;
	std::string _message;

private:
	std::stacktrace _stacktrace;
};

} // namespace arch
