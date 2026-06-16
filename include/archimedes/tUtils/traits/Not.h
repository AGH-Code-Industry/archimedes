#pragma once

namespace arch::traits {

template<template<class...> class Trait>
struct Not {
	template<class... Ts>
	struct type {
		static constexpr bool value = !Trait<Ts...>::value;
	};
};

} // namespace arch::traits
