#pragma once

#include <exception>

#include <archimedes/Logger.h>
#include <archimedes/utils/SimpleSourceLocation.h>

namespace arch {

/// @brief Exception class
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

	/// @brief Returns exception location
	const utils::SimpleSourceLocation& location() const noexcept;

	/// @brief Returns exception stacktrace
	const std::stacktrace& stacktrace() const noexcept;

	/// @brief Adds stacktrace to this exception and returns it
	/// @details Be aware, that building stacktrace is an expensive operation
	template<class Self>
	auto&& withStacktrace(this Self&& self, const std::stacktrace stacktrace = std::stacktrace::current());

protected:
	std::string _title;
	std::string _message;

private:
	union {
		utils::SimpleSourceLocation _location;
		std::stacktrace _stacktrace;
	};

	bool _hasStacktrace;
};

} // namespace arch

#include "Exception.hpp"
