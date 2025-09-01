#include "ReadonlyCounter.h"

template<class T>
T arch::utils::ReadonlyCounter<T>::count() const noexcept {
	return _counter;
}
