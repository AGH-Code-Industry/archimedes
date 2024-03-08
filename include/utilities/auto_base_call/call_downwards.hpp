#pragma once

#include <utilities/register_base.hpp>

#define DOWNWARD_CALL(name, body) \
private: \
	void __downward_call_##name body \
	template<class T, class... Rest> \
	void __downward_caller_of_##name(const std::tuple<T, Rest...>* = nullptr) { \
		if constexpr (sizeof...(Rest) + 1 == std::tuple_size_v<base_t>) { \
			__downward_call_##name(); \
		} \
		T::name(); \
		if constexpr (sizeof...(Rest) != 0) { \
			__downward_caller_of_##name<Rest...>(); \
		} \
	} \
public: \
	virtual void name() { \
		__downward_caller_of_##name((base_t*)(0)); \
	} 