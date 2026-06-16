#include <memory>

#include "RefRes.h"

namespace arch::utils {

template<class T, class E>
RefRes<T, E>::RefRes(T& val) noexcept: _union{ .val = &val }, _hasValue{ true } {}

template<class T, class E>
RefRes<T, E>::RefRes(const Err<E>& val) noexcept: _union{ .err = val._error }, _hasValue{ false } {}

template<class T, class E>
RefRes<T, E>::~RefRes() noexcept {
	if (_hasValue) {
		std::destroy_at(&_union.val);
	} else {
		_union.err.~E();
	}
}

template<class T, class E>
RefRes<T, E>& RefRes<T, E>::operator=(T& val) noexcept {
	_union.val = &val;
	_hasValue = true;
	return *this;
}

template<class T, class E>
RefRes<T, E>& RefRes<T, E>::operator=(const Err<E>& val) noexcept {
	_union.err = val._error;
	_hasValue = false;
	return *this;
}

template<class T, class E>
T* RefRes<T, E>::operator->() noexcept {
	ARCH_ASSERT(_hasValue, "RefRes has no value");
	return _union.val;
}

template<class T, class E>
const T* RefRes<T, E>::operator->() const noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "RefRes has no value");
	return _union.val;
}

template<class T, class E>
T& RefRes<T, E>::operator*() noexcept {
	ARCH_ASSERT(_hasValue, "RefRes has no value");
	return *_union.val;
}

template<class T, class E>
const T& RefRes<T, E>::operator*() const noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "RefRes has no value");
	return *_union.val;
}

template<class T, class E>
T& RefRes<T, E>::value() noexcept {
	ARCH_ASSERT(_hasValue, "RefRes has no value");
	return *_union.val;
}

template<class T, class E>
const T& RefRes<T, E>::value() const noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "RefRes has no value");
	return *_union.val;
}

template<class T, class E>
RefRes<T, E>::operator T&() noexcept {
	ARCH_ASSERT(_hasValue, "RefRes has no value");
	return *_union.val;
}

template<class T, class E>
RefRes<T, E>::operator const T&() const noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "RefRes has no value");
	return *_union.val;
}

template<class T, class E>
T& RefRes<T, E>::tryValue() {
	if (!_hasValue) {
		throw BadResAccess("RefRes has no value");
	}
	return *_union.val;
}

template<class T, class E>
const T& RefRes<T, E>::tryValue() const requires(!isConst)
{
	if (!_hasValue) {
		throw BadResAccess("RefRes has no value");
	}
	return *_union.val;
}

template<class T, class E>
E RefRes<T, E>::error() const noexcept {
	ARCH_ASSERT(!_hasValue, "RefRes has value");
	return _union.err;
}

template<class T, class E>
E RefRes<T, E>::tryError() const {
	if (_hasValue) {
		throw BadResAccess("RefRes has value");
	}
	return _union.err;
}

template<class T, class E>
T& RefRes<T, E>::valueOr(T& other) noexcept {
	return _hasValue ? value() : other;
}

template<class T, class E>
const T& RefRes<T, E>::valueOr(const T& other) const noexcept requires(!isConst)
{
	return _hasValue ? value() : other;
}

template<class T, class E>
E RefRes<T, E>::errorOr(E other) const noexcept {
	return _hasValue ? other : error();
}

template<class T, class E>
bool RefRes<T, E>::hasValue() const noexcept {
	return _hasValue;
}

template<class T, class E>
RefRes<T, E>::operator bool() const noexcept {
	return _hasValue;
}

template<class T, class E>
bool RefRes<T, E>::hasError() const noexcept {
	return !_hasValue;
}

} // namespace arch::utils
