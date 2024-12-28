#pragma once

#include <optional>

namespace arch {

/// @brief std::optional equivalent for references
template<class T>
class OptRef {
public:

	OptRef() noexcept = default;
	OptRef(std::nullopt_t) noexcept;
	OptRef(const OptRef&) noexcept = default;
	OptRef(OptRef&&) noexcept = default;
	OptRef(const OptRef<std::remove_const_t<T>>& other) noexcept requires(std::is_const_v<T>);
	OptRef(OptRef<std::remove_const_t<T>>&& other) noexcept requires(std::is_const_v<T>);
	OptRef(T& ref) noexcept;

	T& operator*() noexcept;
	const T& operator*() const noexcept requires(!std::is_const_v<T>);
	operator T&() noexcept;
	operator const T&() const noexcept requires(!std::is_const_v<T>);
	T& get() noexcept;
	const T& get() const noexcept requires(!std::is_const_v<T>);
	T* operator->() noexcept;
	const T* operator->() const noexcept requires(!std::is_const_v<T>);

	operator bool() const noexcept;
	bool hasValue() const noexcept;

	T& tryGet();
	const T& tryGet() const requires(!std::is_const_v<T>);
	T& getOr(T& other) noexcept;
	const T& getOr(const T& other) const noexcept requires(!std::is_const_v<T>);

	void swap(OptRef& other) noexcept;

private:

	template<class T2>
	friend class OptRef;

	T* _ptr = nullptr;
};

} // namespace arch

#include "OptRef.hpp"
