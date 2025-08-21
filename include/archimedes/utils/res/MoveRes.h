#pragma once

#include <concepts>

#include "Err.h"
#include <utils/MoveFlag.h>

namespace arch::utils {

template<class T, class E>
class MoveRes {
public:

	static constexpr bool isConst = std::is_const_v<T>;

	MoveRes() noexcept = default;
	MoveRes(const MoveRes&) noexcept = default;
	MoveRes(MoveRes&&) noexcept = default;
	MoveRes(const T& val) noexcept requires(!isConst);
	MoveRes(T&& val) noexcept;
	template<class... Args>
	requires(std::constructible_from<T, Args...>)
	MoveRes(Args&&... args) noexcept;
	MoveRes(const Err<E>& val) noexcept;
	~MoveRes() noexcept;

	MoveRes& operator=(const MoveRes&) noexcept = default;
	MoveRes& operator=(MoveRes&&) noexcept = default;
	MoveRes& operator=(const T& val) noexcept requires(!isConst);
	MoveRes& operator=(T&& val) noexcept;
	MoveRes& operator=(const Err<E>& val) noexcept;

	T* operator->() noexcept;
	const T* operator->() const noexcept requires(!isConst);

	T&& operator*() noexcept;
	const T& operator*() const noexcept requires(!isConst);

	T&& value() noexcept;
	const T& value() const noexcept requires(!isConst);

	T&& tryValue();
	const T& tryValue() const requires(!isConst);

	E error() const noexcept;
	E tryError() const;

	T&& valueOr(T& other) noexcept;
	T&& valueOr(T&& other) noexcept;
	const T& valueOr(const T& other) const noexcept requires(!isConst);

	E errorOr(E other) const noexcept;

	bool hasValue() const noexcept;
	bool hasError() const noexcept;
	operator bool() const noexcept;

private:

	union Union {
		T val;
		E err;

		~Union() noexcept {};
	} _union = E();

	bool _hasValue = false;
};

} // namespace arch::utils

#include "MoveRes.hpp"

namespace arch {

using utils::MoveRes;

}
