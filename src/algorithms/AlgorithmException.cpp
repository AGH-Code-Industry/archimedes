#include <archimedes/algorithms/AlgorithmException.h>

namespace arch::algorithms {
AlgorithmException::AlgorithmException(const std::string& message, const utils::SimpleSourceLocation& loc):
	Exception("Algorithm", message, loc) {}
} // namespace arch::math
