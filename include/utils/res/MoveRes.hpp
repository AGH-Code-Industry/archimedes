#include "MoveRes.h"

namespace arch::utils {

template<class T, class E>
MoveRes<T, E>::MoveRes(const T& val) noexcept requires(!isConst)
	: _union{ .val = val }, _hasValue{ true } {}

template<class T, class E>
MoveRes<T, E>::MoveRes(T&& val) noexcept: _union{ .val = val }, _hasValue{ true } {}

template<class T, class E>
template<class... Args>
requires(std::constructible_from<T, Args...>)
MoveRes<T, E>::MoveRes(Args&&... args) noexcept: _union{ .val = T(std::forward<Args>(args)...) }, _hasValue{ true } {}

template<class T, class E>
MoveRes<T, E>::MoveRes(const Err<E>& val) noexcept: _union{ .err = val._error }, _hasValue{ false } {}

template<class T, class E>
MoveRes<T, E>::~MoveRes() noexcept {
	if (_hasValue) {
		_union.val.~T();
	} else {
		_union.err.~E();
	}
}

template<class T, class E>
MoveRes<T, E>& MoveRes<T, E>::operator=(const T& val) noexcept requires(!isConst)
{
	_union.val = val;
	_hasValue = true;
	return *this;
}

template<class T, class E>
MoveRes<T, E>& MoveRes<T, E>::operator=(T&& val) noexcept {
	_union.val = val;
	_hasValue = true;
	return *this;
}

template<class T, class E>
MoveRes<T, E>& MoveRes<T, E>::operator=(const Err<E>& val) noexcept {
	_union.err = val._error;
	_hasValue = false;
	return *this;
}

template<class T, class E>
T* MoveRes<T, E>::operator->() noexcept {
	ARCH_ASSERT(_hasValue, "MoveRes has no value");
	return &_union.val;
}

template<class T, class E>
const T* MoveRes<T, E>::operator->() const noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "MoveRes has no value");
	return &_union.val;
}

template<class T, class E>
T&& MoveRes<T, E>::operator*() noexcept {
	ARCH_ASSERT(_hasValue, "MoveRes has no value");
	return std::move(_union.val);
}

template<class T, class E>
const T& MoveRes<T, E>::operator*() const noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "MoveRes has no value");
	return _union.val;
}

template<class T, class E>
T&& MoveRes<T, E>::value() noexcept {
	ARCH_ASSERT(_hasValue, "MoveRes has no value");
	return std::move(_union.val);
}

template<class T, class E>
const T& MoveRes<T, E>::value() const noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "MoveRes has no value");
	return _union.val;
}

template<class T, class E>
T&& MoveRes<T, E>::tryValue() {
	if (!_hasValue) {
		throw BadResAccess("MoveRes has no value");
	}
	return std::move(_union.val);
}

template<class T, class E>
const T& MoveRes<T, E>::tryValue() const requires(!isConst)
{
	if (!_hasValue) {
		throw BadResAccess("MoveRes has no value");
	}
	return _union.val;
}

template<class T, class E>
E MoveRes<T, E>::error() const noexcept {
	ARCH_ASSERT(!_hasValue, "MoveRes has value");
	return _union.err;
}

template<class T, class E>
E MoveRes<T, E>::tryError() const {
	if (_hasValue) {
		throw BadResAccess("MoveRes has value");
	}
	return _union.err;
}

template<class T, class E>
T&& MoveRes<T, E>::valueOr(T& other) noexcept {
	return std::move(_hasValue ? _union.val : other);
}

template<class T, class E>
T&& MoveRes<T, E>::valueOr(T&& other) noexcept {
	return std::move(_hasValue ? _union.val : other);
}

template<class T, class E>
const T& MoveRes<T, E>::valueOr(const T& other) const noexcept requires(!isConst)
{
	return _hasValue ? _union.val : other;
}

template<class T, class E>
E MoveRes<T, E>::errorOr(E other) const noexcept {
	return _hasValue ? other : _union.err;
}

template<class T, class E>
bool MoveRes<T, E>::hasValue() const noexcept {
	return _hasValue;
}

template<class T, class E>
bool MoveRes<T, E>::hasError() const noexcept {
	return !_hasValue;
}

template<class T, class E>
MoveRes<T, E>::operator bool() const noexcept {
	return _hasValue;
}

} // namespace arch::utils
