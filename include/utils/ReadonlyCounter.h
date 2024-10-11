#pragma once

namespace arch::utils {

/// @brief Readonly counter of given type, derived class manages _counter field.
/// @tparam T - type of _counter field
template<class T>
class ReadonlyCounter {
public:
	/// @brief Returns value of _counter
	T count() const noexcept;

protected:
	T _counter{};
};
extern template class ReadonlyCounter<size_t>;

} // namespace arch::utils

#include "ReadonlyCounter.hpp"
