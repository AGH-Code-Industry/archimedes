#pragma once

#include <concepts>

#include "Err.h"
#include "utils/MoveFlag.h"

namespace arch::utils {

template<class T, class E>
class RefRes {
public:

	static constexpr bool isConst = std::is_const_v<T>;

	RefRes() noexcept = default;
	RefRes(const RefRes&) noexcept = default;
	RefRes(RefRes&&) noexcept = default;
	RefRes(T& val) noexcept;
	RefRes(const Err<E>& val) noexcept;
	~RefRes() noexcept;

	RefRes& operator=(const RefRes&) noexcept = default;
	RefRes& operator=(RefRes&&) noexcept = default;
	RefRes& operator=(T& val) noexcept;
	RefRes& operator=(const Err<E>& val) noexcept;

	T* operator->() noexcept;
	const T* operator->() const noexcept requires(!isConst);

	T& operator*() noexcept;
	const T& operator*() const noexcept requires(!isConst);

	T& value() noexcept;
	const T& value() const noexcept requires(!isConst);

	operator T&() noexcept;
	operator const T&() const noexcept requires(!isConst);

	T& tryValue();
	const T& tryValue() const requires(!isConst);

	E error() const noexcept;
	E tryError() const;

	T& valueOr(T& other) noexcept;
	const T& valueOr(const T& other) const noexcept requires(!isConst);
	E errorOr(E other) const noexcept;

	bool hasValue() const noexcept;
	bool hasError() const noexcept;
	operator bool() const noexcept;

private:

	union Union {
		T* val;
		E err;

		~Union() noexcept {};
	} _union = E();

	bool _hasValue = false;
};

} // namespace arch::utils

#include "RefRes.hpp"

namespace arch {

using utils::RefRes;

}
