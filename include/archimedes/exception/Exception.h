#pragma once

#include <concepts>
#include <exception>
#include <variant>

#include <archimedes/Logger.h>
#include <archimedes/utils/OptRef.h>
#include <archimedes/utils/SimpleSourceLocation.h>

namespace arch {

/// @brief Exception class
/// @details Captures source location by default, but can optionally capture stacktrace
/// @details Exception which captured stacktrace loses source location
class Exception: public std::exception {
public:
	/// @brief Constructor
	/// @param title - exception title
	/// @param message - exception message
	/// @param location - exception location (use utils::SimpleSourceLocation::current() for default value)
	Exception(
		std::string_view title,
		std::string_view message,
		const utils::SimpleSourceLocation& location = utils::SimpleSourceLocation::current()
	);

	/// @brief Inherited from std::exception
	/// @return message()
	const char* what() const noexcept override final;

	/// @brief Returns exception title
	std::string_view title() const noexcept;

	/// @brief Returns exception message
	std::string_view message() const noexcept;

	/// @brief Whether exception has stacktrace
	/// @brief true implies lack of source location
	bool hasStacktrace() const noexcept;

	/// @brief Returns exception location
	/// @details Unavailable is exception captured stacktrace
	OptRef<const utils::SimpleSourceLocation> location() const noexcept;

	/// @brief Returns exception stacktrace
	OptRef<const std::stacktrace> stacktrace() const noexcept;

	/// @brief Adds stacktrace to this exception and returns it
	/// @details Be aware, that building stacktrace is an expensive operation
	template<class Self>
	auto&& withStacktrace(this Self&& self, std::stacktrace stacktrace = std::stacktrace::current());

protected:
	std::string _title;
	std::string _message;

private:
	std::variant<utils::SimpleSourceLocation, std::stacktrace> _locationOrStacktrace;
};

} // namespace arch

#include "Exception.hpp"
