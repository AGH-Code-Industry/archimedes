#pragma once

#include <string>
#include <compare>
#include <template_utilities/nameoftype.hpp>
#include <serialization/hash.hpp>
#include <iostream>

namespace arch {
template<class T>
class TypeDescriptor;

/// @brief Wrapper to TypeID. Obtained by calling arch::TypeDescriptor<T>::get() or TypeDesc(T).
///
class TypeDescriptorWrapper {
public:
	/// @brief Sets comparision precision.
	/// @param new_precision - new value of precision, if negative compares whole names.
	static void precision(int new_precision) noexcept;
	/// @brief Returns comparision precision (how long should be substring of name to compare).
	///
	static int precision() noexcept;

	/// @brief Returns formatted name of type, equal among implementations.
	///  
	const std::string& name() const noexcept;
	/// @brief Returns hash of type descriptor.
	/// 
	size_t hash() const noexcept;
	/// @brief Returns size of type.
	///
	size_t size() const noexcept;
	/// @brief Returns alignment of type.
	///
	size_t alignment() const noexcept;

	/// @brief Checks if two wrappers point for the same type.
	/// @brief First compares hashes, then random substrings from names.
	/// @param other - wrapper to compare to.
	bool operator==(const TypeDescriptorWrapper& other) const noexcept;
	/// @brief Compares two wrappers (lexicographically by name).
	/// @param other - wrapper to compare to.
	std::strong_ordering operator<=>(const TypeDescriptorWrapper& other) const noexcept;

private:
	template<class T>
	friend class arch::TypeDescriptor;

	friend std::unique_ptr<TypeDescriptorWrapper> std::make_unique<TypeDescriptorWrapper>();

	TypeDescriptorWrapper(const std::string& _n, size_t _h, size_t _s, size_t _a);

	const std::string& _name;
	size_t _hash;
	size_t _size;
	size_t _alignment;

	static int _precision;
};

/// @brief Class representing runtime type descriptor.
/// @tparam T - any type
template<class T>
class TypeDescriptor {
public:
	/// @brief Returns formatted name of type, equal among compilers and systems.
	/// 
	static const std::string& name() noexcept;
	/// @brief Returns hash of type descriptor.
	/// 
	static size_t hash() noexcept;
	/// @brief Returns size of type.
	///
	static size_t size() noexcept;
	/// @brief Returns alignment of type.
	///
	static size_t alignment() noexcept;

	/// @brief Returns reference to wrapper of this TypeDescriptor.
	///
	static const TypeDescriptorWrapper& get() noexcept;

private:
	static inline std::string _name;// = tmpl_utils::nameoftype_fmt(tmpl_utils::nameoftype<T>());
	static inline size_t _hash;// = arch::ser::hash(name());
	static inline constexpr size_t _size = sizeof(T);
	static inline constexpr size_t _alignment = alignof(T);

	static inline std::unique_ptr<TypeDescriptorWrapper> _wrapper;
};

template<class T>
const std::string& TypeDescriptor<T>::name() noexcept {
	if (_name.empty()) {
		_name = tmpl_utils::nameoftype_fmt(tmpl_utils::nameoftype<T>());
		_hash = arch::ser::hash(name());
	}
	return _name;
}
template<class T>
size_t TypeDescriptor<T>::hash() noexcept {
	if (_name.empty()) {
		_name = tmpl_utils::nameoftype_fmt(tmpl_utils::nameoftype<T>());
		_hash = arch::ser::hash(name());
	}
	return _hash;
}
template<class T>
size_t TypeDescriptor<T>::size() noexcept {
	return _size;
}
template<class T>
size_t TypeDescriptor<T>::alignment() noexcept {
	return _alignment;
}
template<class T>
const TypeDescriptorWrapper& TypeDescriptor<T>::get() noexcept {
	if (_wrapper.get() == nullptr) {
		_wrapper = std::unique_ptr<TypeDescriptorWrapper>(new TypeDescriptorWrapper(name(), hash(), size(), alignment()));
	}
	//TypeDescriptorWrapper wrapper{name(), hash(), size(), alignment()};
	return *_wrapper;
}
}

template<>
struct std::hash<arch::TypeDescriptorWrapper> {
	size_t operator()(const arch::TypeDescriptorWrapper& x) const noexcept {
		return x.hash();
	}
};


/// @brief Obtains TypeDescriptor.
///
#define typedesc(T) arch::TypeDescriptor<T>::get()