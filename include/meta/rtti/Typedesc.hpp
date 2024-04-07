#pragma once

#include "TypeDescriptor.h"

#define _typedescImpl(T) arch::meta::rtti::TypeDescriptorOwner<T>::get()
/// @brief Queries TypeDescriptor of given typ
/// @param T - type which descriptor to access
#define typedesc(T) _typedescImpl(T)
