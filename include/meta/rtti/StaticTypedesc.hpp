#pragma once

#include <meta/Typeof.hpp>

#include "TypeDescriptor.h"
#include "TypeDescriptorOwner.h"

#define _staticTypedescImpl(...) arch::meta::rtti::TypeDescriptorOwner<typeof(__VA_ARGS__)>::get()

/// @brief Queries TypeDescriptor of given type, without usage of polymorphism
/// @param ... - type/expression which TypeDescriptor to access
#define static_typedesc(...) _staticTypedescImpl(__VA_ARGS__)
