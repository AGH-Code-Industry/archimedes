#include "OptRef.h"

namespace arch {

template<class T>
OptRef<T>::OptRef(std::nullopt_t) noexcept: OptRef() {}

template<class T>
OptRef<T>::OptRef(T& ref) noexcept: _ptr{ &ref } {}

template<class T>
T& OptRef<T>::operator*() noexcept {
	return *_ptr;
}

template<class T>
const T& OptRef<T>::operator*() const noexcept requires(!std::is_const_v<T>)
{
	return *_ptr;
}

template<class T>
OptRef<T>::operator T&() noexcept {
	return *_ptr;
}

template<class T>
OptRef<T>::operator const T&() const noexcept requires(!std::is_const_v<T>)
{
	return *_ptr;
}

template<class T>
bool OptRef<T>::hasValue() const noexcept {
	return _ptr != nullptr;
}

template<class T>
OptRef<T>::operator bool() const noexcept {
	return _ptr != nullptr;
}

template<class T>
T& OptRef<T>::get() noexcept {
	return *_ptr;
}

template<class T>
const T& OptRef<T>::get() const noexcept requires(!std::is_const_v<T>)
{
	return *_ptr;
}

template<class T>
T& OptRef<T>::tryGet() {
	if (!_ptr) {
		throw std::bad_optional_access();
	}
	return *_ptr;
}

template<class T>
const T& OptRef<T>::tryGet() const requires(!std::is_const_v<T>)
{
	if (!_ptr) {
		throw std::bad_optional_access();
	}
	return *_ptr;
}

template<class T>
T& OptRef<T>::getOr(T& other) noexcept {
	if (!_ptr) {
		return other;
	}
	return *_ptr;
}

template<class T>
const T& OptRef<T>::getOr(const T& other) const noexcept requires(!std::is_const_v<T>)
{
	if (!_ptr) {
		return other;
	}
	return *_ptr;
}

template<class T>
void OptRef<T>::swap(OptRef& other) noexcept {
	std::swap(_ptr, other._ptr);
}

} // namespace arch

