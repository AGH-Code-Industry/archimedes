#include <serialization/exception.hpp>

namespace arch::ser {
SerializationException::SerializationException(std::string title) :
	Exception("Serialization error: " + title) {}
}