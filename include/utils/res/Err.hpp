#include "Err.h"

namespace arch::utils {

template<class E>
Err<E>::Err(const E& error) noexcept: _error{ error } {}

template<class E>
Err<E>& Err<E>::operator=(const E& error) noexcept {
	_error = error;
	return *this;
}

} // namespace arch::utils
