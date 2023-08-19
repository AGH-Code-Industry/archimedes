#pragma once

#include <memory>
#include "serialized.hpp"

namespace arch::ser {
/// @brief Interface to enable serialization.
/// @brief Derived class must override serialize() method to be Serializable.
struct ISerializable {
	/// @brief Serializing method.
	/// @brief While specializing it is required to use special Serialzed::new_serialized<T>() template constructor.
	/// @return Serialized object.
	virtual Serialized serialize() const = 0;
};
}

/// @brief Default in-class serialization method.
///
#define DEFUALT_SERIALIZE() \
virtual arch::ser::Serialized serialize() const { \
	return arch::ser::Serialized::new_serialized<std::remove_cvref_t<decltype(*this)>>(memcpy(malloc(sizeof(decltype(*this))), this, sizeof(decltype(*this)))); \
}

/// @brief Default serialization method declaration.
///
#define DEFAULT_SERIALIZE_DECL() \
virtual arch::ser::Serialized serialize() const

/// @brief Default outside of class serialization method definition.
/// @param T - class to define.
#define DEFAULT_SERIALIZE_OUTSIDE(T) \
arch::ser::Serialized T::serialize() const { \
	return arch::ser::Serialized::new_serialized<std::remove_cvref_t<decltype(*this)>>(memcpy(malloc(sizeof(decltype(*this))), this, sizeof(decltype(*this)))); \
}