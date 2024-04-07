#pragma once

#include "TypeDescriptor.h"

#define _typedescImpl(T) arch::meta::rtti::TypeDescriptor<T>::get()
#define typedesc(T) _typedescImpl(T)
