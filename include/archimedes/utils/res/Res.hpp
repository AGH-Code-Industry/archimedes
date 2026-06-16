#include "BadResAccess.h"
#include "Res.h"

namespace arch::utils {

template<class T, class E>
Res<T, E>::Res(const T& val) noexcept requires(!isConst)
	: _union{ .val = val }, _hasValue{ true } {}

template<class T, class E>
Res<T, E>::Res(T&& val) noexcept: _union{ .val = val }, _hasValue{ true } {}

template<class T, class E>
template<class... Args>
requires(std::constructible_from<T, Args...>)
Res<T, E>::Res(Args&&... args) noexcept: _union{ .val = T(std::forward<Args>(args)...) }, _hasValue{ true } {}

template<class T, class E>
Res<T, E>::Res(const Err<E>& val) noexcept: _union{ .err = val._error }, _hasValue{ false } {}

template<class T, class E>
Res<T, E>::~Res() noexcept {
	if (_hasValue) {
		_union.val.~T();
	} else {
		_union.err.~E();
	}
}

template<class T, class E>
Res<T, E>& Res<T, E>::operator=(const T& val) noexcept requires(!isConst)
{
	_union.val = val;
	_hasValue = true;
	return *this;
}

template<class T, class E>
Res<T, E>& Res<T, E>::operator=(T&& val) noexcept {
	_union.val = val;
	_hasValue = true;
	return *this;
}

template<class T, class E>
Res<T, E>& Res<T, E>::operator=(const Err<E>& val) noexcept {
	_union.err = val._error;
	_hasValue = false;
	return *this;
}

template<class T, class E>
T* Res<T, E>::operator->() noexcept {
	ARCH_ASSERT(_hasValue, "Res has no value");
	return &_union.val;
}

template<class T, class E>
const T* Res<T, E>::operator->() const noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "Res has no value");
	return &_union.val;
}

template<class T, class E>
T& Res<T, E>::operator*() & noexcept {
	ARCH_ASSERT(_hasValue, "Res has no value");
	return _union.val;
}

template<class T, class E>
const T& Res<T, E>::operator*() const& noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "Res has no value");
	return _union.val;
}

template<class T, class E>
T&& Res<T, E>::operator*() && noexcept {
	ARCH_ASSERT(_hasValue, "Res has no value");
	return std::move(_union.val);
}

template<class T, class E>
T& Res<T, E>::value() & noexcept {
	ARCH_ASSERT(_hasValue, "Res has no value");
	return _union.val;
}

template<class T, class E>
T&& Res<T, E>::value(MoveFlag) & noexcept {
	ARCH_ASSERT(_hasValue, "Res has no value");
	return std::move(_union.val);
}

template<class T, class E>
const T& Res<T, E>::value() const& noexcept requires(!isConst)
{
	ARCH_ASSERT(_hasValue, "Res has no value");
	return _union.val;
}

template<class T, class E>
T&& Res<T, E>::value(MoveFlag) && noexcept {
	ARCH_ASSERT(_hasValue, "Res has no value");
	return std::move(_union.val);
}

template<class T, class E>
T& Res<T, E>::tryValue() & {
	if (!_hasValue) {
		throw BadResAccess("Res has no value");
	}
	return _union.val;
}

template<class T, class E>
T&& Res<T, E>::tryValue(MoveFlag) & {
	if (!_hasValue) {
		throw BadResAccess("Res has no value");
	}
	return std::move(_union.val);
}

template<class T, class E>
const T& Res<T, E>::tryValue() const& requires(!isConst)
{
	if (!_hasValue) {
		throw BadResAccess("Res has no value");
	}
	return _union.val;
}

template<class T, class E>
T&& Res<T, E>::tryValue(MoveFlag) && {
	if (!_hasValue) {
		throw BadResAccess("Res has no value");
	}
	return std::move(_union.val);
}

template<class T, class E>
E Res<T, E>::error() const noexcept {
	ARCH_ASSERT(!_hasValue, "Res has value");
	return _union.err;
}

template<class T, class E>
E Res<T, E>::tryError() const {
	if (_hasValue) {
		throw BadResAccess("Res has value");
	}
	return _union.err;
}

template<class T, class E>
T& Res<T, E>::valueOr(T& other) & noexcept {
	return _hasValue ? _union.val : other;
}

template<class T, class E>
T&& Res<T, E>::valueOr(T& other, MoveFlag) & noexcept {
	return std::move(_hasValue ? _union.val : other);
}

template<class T, class E>
T&& Res<T, E>::valueOr(T&& other, MoveFlag) & noexcept {
	return std::move(_hasValue ? _union.val : other);
}

template<class T, class E>
const T& Res<T, E>::valueOr(const T& other) const& noexcept requires(!isConst)
{
	return _hasValue ? _union.val : other;
}

template<class T, class E>
T&& Res<T, E>::valueOr(T& other, MoveFlag) && noexcept {
	return std::move(_hasValue ? _union.val : other);
}

template<class T, class E>
T&& Res<T, E>::valueOr(T&& other, MoveFlag) && noexcept {
	return std::move(_hasValue ? _union.val : other);
}

template<class T, class E>
E Res<T, E>::errorOr(E other) const noexcept {
	return _hasValue ? other : _union.err;
}

template<class T, class E>
bool Res<T, E>::hasValue() const noexcept {
	return _hasValue;
}

template<class T, class E>
bool Res<T, E>::hasError() const noexcept {
	return !_hasValue;
}

template<class T, class E>
Res<T, E>::operator bool() const noexcept {
	return _hasValue;
}

template<class E>
Res<void, E>::Res(const Err<E>& val) noexcept: _err{ val._error }, _hasValue{ false } {}

template<class E>
Res<void, E>& Res<void, E>::operator=(const Err<E>& val) noexcept {
	_err = val._error;
	_hasValue = false;
	return *this;
}

template<class E>
bool Res<void, E>::operator*() const noexcept {
	return _hasValue;
}

template<class E>
bool Res<void, E>::value() const noexcept {
	return _hasValue;
}

template<class E>
E Res<void, E>::error() const noexcept {
	ARCH_ASSERT(!_hasValue, "Res has value");
	return _err;
}

template<class E>
E Res<void, E>::tryError() const {
	if (_hasValue) {
		throw BadResAccess("Res has value");
	}
	return _err;
}

template<class E>
E Res<void, E>::errorOr(E other) const noexcept {
	return _hasValue ? other : _err;
}

template<class E>
bool Res<void, E>::hasValue() const noexcept {
	return _hasValue;
}

template<class E>
bool Res<void, E>::hasError() const noexcept {
	return !_hasValue;
}

template<class E>
Res<void, E>::operator bool() const noexcept {
	return _hasValue;
}

} // namespace arch::utils
