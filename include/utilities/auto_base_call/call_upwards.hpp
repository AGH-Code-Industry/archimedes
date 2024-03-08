#pragma once

#include <utilities/register_base.hpp>

#define UPWARD_CALL(name, body) \
private: \
	void __upward_call_##name body \
	template<class T, class... Rest> \
	void __upward_caller_of_##name(const std::tuple<T, Rest...>* = nullptr) { \
		T::name(); \
		if constexpr (sizeof...(Rest) == 0) { \
			__upward_call_##name(); \
		} \
		else { \
			__upward_caller_of_##name<Rest...>(); \
		} \
	} \
public: \
	virtual void name() { \
		__upward_caller_of_##name((base_t*)(0)); \
	} 