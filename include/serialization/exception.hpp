#pragma once

#include <exception.h>

namespace arch::ser {
/// @brief Exception for serializations.
///
class SerializationException : public arch::Exception {
public:
	SerializationException(std::string title);
};
/// @brief Exception type alias.
///
using Exception = SerializationException;
}