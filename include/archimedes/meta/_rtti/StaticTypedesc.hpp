#pragma once

#include <archimedes/meta/Typeof.hpp>

#include "TypeDescriptor.h"
#include "TypeDescriptorOwner.h"

#define _ARCH_STATIC_TYPEDESC_IMPL(...) arch::meta::rtti::TypeDescriptorOwner<typeof(__VA_ARGS__)>::get()

/// @brief Queries TypeDescriptor of given type, without usage of polymorphism
/// @param ... - type/expression which TypeDescriptor to access
#define staticTypedesc(...) _ARCH_STATIC_TYPEDESC_IMPL(__VA_ARGS__)
