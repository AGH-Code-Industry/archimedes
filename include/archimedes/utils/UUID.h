#pragma once

#include <array>
#include <string>

#include <ArchMath.h>
#include <net/GetMac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

namespace arch::utils {

/// @brief Universally Unique Identifier
/// @details Compliant with RFC 9562
class UUID {
public:
	/// @brief Variant value indicating compliance with RFC 9562
	static constexpr u32 variantRFCCompliant = 0b10;

	/// @brief Versions based on current time
	enum class TimeBased : u8 {
		/// @brief Timestamp (since 15 October 1582) is divided into three parts and placed in reversed order
		v1,
		/// @brief Timestamp (since 15 October 1582) is divided into three parts and placed in order
		v6,
		/// @brief Timestamp (since Epoch) is placed in order
		v7
	};

	/// @brief Versions based on specified name (or namespace + name)
	enum class NameBased : u8 {
		/// @brief Uses MD5 algorithm
		v3,
		/// @brief Uses SHA-1 algoritm
		v5
	};

	/// @brief Versions based on randomness
	enum class RandomBased : u8 {
		/// @brief Uses random data instead of MAC address
		v1,
		/// @brief Most of data is randomly generated
		v4,
		/// @brief Uses random data instead of MAC address
		v6,
		/// @brief More than half of data is randomly generated
		v7
	};

	/// @brief Creates UUID from current timestamp
	/// @param uuidVersion - version of UUID to create (default 7)
	/// @param useMAC - whether to use MAC address instead of random data (default true, v7 excluded)
	static UUID fromTime(const TimeBased uuidVersion = TimeBased::v7, const bool useMAC = true) noexcept;
	/// @brief Creates UUID from given name
	/// @details Works as fromNamespacedName with zeroed nameSpace
	/// @param name - string data to use as name
	/// @param uuidVersion - version of UUID (default 5)
	static UUID fromName(std::string_view name, const NameBased uuidVersion = NameBased::v5) noexcept;
	/// @brief Creates UUID from given nameSpace and name
	/// @param nameSpace - UUID of namespace
	/// @param name - string data to use as name
	/// @param uuidVersion - version of UUID (default 5)
	static UUID fromNamespacedName(
		const UUID& nameSpace,
		std::string_view name,
		const NameBased uuidVersion = NameBased::v5
	) noexcept;
	/// @brief Creates (at least partially) random UUID
	/// @param uuidVersion - version of UUID (default 4)
	static UUID random(const RandomBased uuidVersion = RandomBased::v4) noexcept;
	/// @brief Creates UUID from custom data
	/// @param data - 16 bytes to copy to UUID
	static UUID fromData(const std::array<u8, 16>& data) noexcept;
	/// @brief Creates UUID from string in format:
	/// @brief xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
	/// @param str - string to create UUID from
	static UUID fromString(std::string_view str) noexcept;

	/// @brief Creates UUID version 1
	/// @param useMAC - whether to use MAC address instead of random data (default true)
	static UUID v1(const bool useMAC = true) noexcept;
	/// @brief Creates UUID version 3, using MD5 algorithm
	/// @details Works as v3 with zeroed nameSpace
	/// @param name - string data to use as name
	static UUID v3(std::string_view name) noexcept;
	/// @brief Creates UUID version 3, using MD5 algorithm
	/// @param nameSpace - UUID of namespace
	/// @param name - string data to use as name
	static UUID v3(const UUID& nameSpace, std::string_view name) noexcept;
	/// @brief Creates UUID version 4
	static UUID v4() noexcept;
	/// @brief Creates UUID version 5, using SHA-1 algorithm
	/// @details Works as v3 with zeroed nameSpace
	/// @param name - string data to use as name
	static UUID v5(std::string_view name) noexcept;
	/// @brief Creates UUID version 5, using SHA-1 algorithm
	/// @param nameSpace - UUID of namespace
	/// @param name - string data to use as name
	static UUID v5(const UUID& nameSpace, std::string_view name) noexcept;
	/// @brief Creates UUID version 6
	/// @param useMAC - whether to use MAC address instead of random data (default true)
	static UUID v6(const bool useMAC = true) noexcept;
	/// @brief Creates UUID version 7
	static UUID v7() noexcept;
	/// @brief Creates UUID version 8
	/// @param data - 16 bytes to copy to UUID
	static UUID v8(const std::array<u8, 16>& data) noexcept;

	/// @brief Copy constructor
	UUID(const UUID&) noexcept = default;
	/// @brief Move constructor
	UUID(UUID&&) noexcept = default;
	/// @brief Copy-assignment operator
	UUID& operator=(const UUID&) noexcept = default;
	/// @brief Move-assignment operator
	UUID& operator=(UUID&&) noexcept = default;

	/// @brief Returns version of UUID
	/// @details Bits [48, 51]
	u32 version() const noexcept;
	/// @brief Returns variant of UUID
	/// @details Bits 64 and 65
	u32 variant() const noexcept;
	/// @brief Checks if UUID is emtpy
	bool invalid() const noexcept;
	/// @brief Checks if UUID is emtpy
	operator bool() const noexcept;

	/// @brief Equality operator
	bool operator==(const UUID&) const noexcept = default;
	/// @brief Comparision operator
	/// @details compares UUIDs lexicographically
	std::strong_ordering operator<=>(const UUID& other) const noexcept;

	/// @brief Returns readonly reference to data
	const std::array<u8, 16>& data() const noexcept;
	/// @brief Returns UUID's string representation
	std::string toString() const noexcept;

private:

	static const std::array<u8, 6> _cachedMAC;
	static const UUID _cachedInvalid;

	// how many 100ns have passed since midnight of 15 October 1582 (adoption of Gregorian calendar)
	static uint64_t _x100nsSinceGregorian() noexcept;

	enum HashFn {
		md5,
		sha1
	};

	static UUID _fromName(std::string_view name, HashFn hashFn) noexcept;
	static UUID _fromNamespacedName(const UUID& nameSpace, std::string_view name, HashFn hashFn) noexcept;

	UUID() noexcept = default;

	std::array<u8, 16> _data{};
};

} // namespace arch::utils

namespace arch {

using utils::UUID;

}
