#pragma once

#include <exception>
#include <source_location>

#include "Logger.h"

namespace arch {

/// Base class for all exceptions in the project.
/// Do not use this class directly, use derived classes instead. (@see InitException)
///
class Exception: public std::exception {
protected:

	/// Constructor.
	/// @param title Title of the exception.
	/// @param location Source location of the exception.
	///
	Exception(const std::string& title, const std::source_location& location);

	/// Constructor.
	/// @param title Title of the exception.
	/// @param message Message of the exception.
	/// @param location Source location of the exception.
	///
	Exception(const std::string& title, const std::string& message, const std::source_location& location);

public:
	/// print the exception to the console.
	/// @param level Log level of the exception.
	///
	void print(LogLevel level = LogLevel::error) const;

	// Inherited form std::exception Get Exception message.
	/// @return Exception message.
	///
	const char* what() const noexcept override;

	/// Get the title of the exception.
	/// @return Title of the exception.
	///
	std::string_view title() const;

	/// Get the message of the exception.
	/// @return Message of the exception.
	///
	std::string_view message() const;

	/// Get the source location of the exception.
	/// @return Source location of the exception.
	///
	const std::source_location& location() const;

protected:
	std::string _title;
	std::string _message;
	std::source_location _location;
};

} // namespace arch
