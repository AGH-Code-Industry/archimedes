#pragma once

#include <concepts>

#include "Err.h"
#include <archimedes/utils/MoveFlag.h>

namespace arch::utils {

template<class T, class E>
class Res {
public:

	static constexpr bool isConst = std::is_const_v<T>;

	Res() noexcept = default;
	Res(const Res&) noexcept = default;
	Res(Res&&) noexcept = default;
	Res(const T& val) noexcept requires(!isConst);
	Res(T&& val) noexcept;
	template<class... Args>
	requires(std::constructible_from<T, Args...>)
	Res(Args&&... args) noexcept;
	Res(const Err<E>& val) noexcept;
	~Res() noexcept;

	Res& operator=(const Res&) noexcept = default;
	Res& operator=(Res&&) noexcept = default;
	Res& operator=(const T& val) noexcept requires(!isConst);
	Res& operator=(T&& val) noexcept;
	Res& operator=(const Err<E>& val) noexcept;

	T* operator->() noexcept;
	const T* operator->() const noexcept requires(!isConst);

	T& operator*() & noexcept;
	const T& operator*() const& noexcept requires(!isConst);
	T&& operator*() && noexcept;

	T& value() & noexcept;
	T&& value(MoveFlag) & noexcept;
	const T& value() const& noexcept requires(!isConst);
	T&& value(MoveFlag = moveFlag) && noexcept;

	T& tryValue() &;
	T&& tryValue(MoveFlag) &;
	const T& tryValue() const& requires(!isConst);
	T&& tryValue(MoveFlag = moveFlag) &&;

	E error() const noexcept;
	E tryError() const;

	T& valueOr(T& other) & noexcept;
	T&& valueOr(T&& other, MoveFlag) & noexcept;
	T&& valueOr(T& other, MoveFlag) & noexcept;
	const T& valueOr(const T& other) const& noexcept requires(!isConst);
	T&& valueOr(T& other, MoveFlag = moveFlag) && noexcept;
	T&& valueOr(T&& other, MoveFlag = moveFlag) && noexcept;

	E errorOr(E other) const noexcept;

	bool hasValue() const noexcept;
	bool hasError() const noexcept;
	operator bool() const noexcept;

private:

	union Union {
		T val;
		E err;

		~Union() noexcept {}
	} _union = E();

	bool _hasValue = false;
};

template<class E>
class Res<void, E> {
public:

	static constexpr bool isConst = false;

	Res() noexcept = default;
	Res(const Res&) noexcept = default;
	Res(Res&&) noexcept = default;
	Res(const Err<E>& val) noexcept;

	Res& operator=(const Res&) noexcept = default;
	Res& operator=(Res&&) noexcept = default;
	Res& operator=(const Err<E>& val) noexcept;

	bool operator*() const noexcept;

	bool value() const noexcept;

	E error() const noexcept;
	E tryError() const;

	E errorOr(E other) const noexcept;

	bool hasValue() const noexcept;
	bool hasError() const noexcept;
	operator bool() const noexcept;

private:

	E _err = {};
	bool _hasValue = true;
};

} // namespace arch::utils

#include "Res.hpp"

namespace arch {

using utils::Res;

}
