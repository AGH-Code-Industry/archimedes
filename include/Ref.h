#pragma once

#include <memory>

namespace arch {

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
requires std::constructible_from<T, Args...>
constexpr Ref<T> createRef(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
requires std::derived_from<T, std::enable_shared_from_this<T>>
constexpr Ref<T> createRef(T* ptr) {
	return ptr->shared_from_this();
}

} // namespace arch
