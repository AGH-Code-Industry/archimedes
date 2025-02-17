#include <algorithm>
#include <charconv>
#include <chrono>
#include <iostream>

#include <net/GetMac.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <utils/UUID.h>

namespace arch::utils {

const std::array<u8, 6> UUID::_cachedMAC = []() {
	std::array<u8, 6> temp;
	net::getMac(temp);
	return temp;
}();

const UUID UUID::_cachedInvalid = {};

UUID UUID::fromTime(const TimeBased uuidVersion, const bool useMAC) noexcept {
	switch (uuidVersion) {
		case TimeBased::v1: return v1(useMAC);
		case TimeBased::v6: return v6(useMAC);
		case TimeBased::v7: return v7();
		default:			return UUID();
	}
}

UUID UUID::fromName(std::string_view name, const NameBased uuidVersion) noexcept {
	switch (uuidVersion) {
		case NameBased::v3: return v3(name);
		case NameBased::v5: return v5(name);
		default:			return UUID();
	}
}

UUID UUID::fromNamespacedName(const UUID& nameSpace, std::string_view name, const NameBased uuidVersion) noexcept {
	switch (uuidVersion) {
		case NameBased::v3: return v3(nameSpace, name);
		case NameBased::v5: return v5(nameSpace, name);
		default:			return UUID();
	}
}

UUID UUID::random(const RandomBased uuidVersion) noexcept {
	switch (uuidVersion) {
		case RandomBased::v1: return v1(false);
		case RandomBased::v4: return v4();
		case RandomBased::v6: return v6(false);
		case RandomBased::v7: return v7();
		default:			  return UUID();
	}
}

UUID UUID::fromData(const std::array<u8, 16>& data) noexcept {
	return v8(data);
}

UUID UUID::fromString(std::string_view str) noexcept {
	std::array<u8, 16> data{};

	if (str.length() < 36) {
		return UUID();
	}

	if (std::sscanf(
			str.data(),
			// clang-format does a little trolling
			"%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%"
			"02hhx",
			&data[0x0],
			&data[0x1],
			&data[0x2],
			&data[0x3],
			&data[0x4],
			&data[0x5],
			&data[0x6],
			&data[0x7],
			&data[0x8],
			&data[0x9],
			&data[0xa],
			&data[0xb],
			&data[0xc],
			&data[0xd],
			&data[0xe],
			&data[0xf]
		) != 16) {
		return UUID();
	}

	UUID result;
	std::copy_n(data.data(), 16, result._data.data());
	return result;
}

uint64_t UUID::_x100nsSinceGregorian() noexcept {
	namespace chr = std::chrono;

	return chr::duration_cast<chr::duration<long long, std::ratio<1, 10'000'000>>>(
			   chr::system_clock::now() - chr::sys_days(chr::day(15) / chr::October / chr::year(1'582))
	)
		.count();

	// ChatGPT did the above in ~30 lines of code
	// you guessed it, the code was computing leap years by hand (wrong results)

	// "Umysł robota walczy z umysłem debila"
	// - Mkbewe, 15 October 2009
}

UUID UUID::v1(const bool useMAC) noexcept {
	UUID result;

	auto time = _x100nsSinceGregorian();

	const auto timeLow = time & 0xffff'ffff;
	// RFC requires network byte order (big endian)
	result._data[0x0] |= timeLow >> 24;
	result._data[0x1] |= timeLow >> 16;
	result._data[0x2] |= timeLow >> 8;
	result._data[0x3] |= timeLow;

	const auto timeMid = (time >> 32) & 0xffff;
	result._data[0x4] |= timeMid >> 8;
	result._data[0x5] |= timeMid;

	const auto timeHigh = (time >> (32 + 16)) & 0x0fff;
	// version | timeHigh (in one byte)
	result._data[0x6] |= (1 << 4) | (timeHigh >> 8);
	result._data[0x7] |= timeHigh;

	uint16_t clockSeq;
	RAND_bytes((u8*)&clockSeq, sizeof(clockSeq));
	clockSeq &= 0x3fff; // retain right 14 bits
	// variant | 6 left bits of clockSeq
	result._data[0x8] |= 0b10000000 | (clockSeq >> 8);
	result._data[0x9] |= clockSeq;

	if (useMAC && std::ranges::all_of(_cachedMAC, [](const u8 byte) { return byte != 0; })) {
		std::copy_n(_cachedMAC.data(), 6, &result._data[0xa]);
	} else {
		// cryptographically safe randomness, required by RFC
		RAND_bytes(&result._data[0xa], 6);
	}

	return result;
}

UUID UUID::v6(const bool useMAC) noexcept {
	UUID result;

	auto time = _x100nsSinceGregorian();

	const auto timeHigh = (time >> 32) & 0xffff'ffff;
	result._data[0x0] |= timeHigh >> 24;
	result._data[0x1] |= timeHigh >> 16;
	result._data[0x2] |= timeHigh >> 8;
	result._data[0x3] |= timeHigh;

	const auto timeMid = (time >> 16) & 0xffff;
	result._data[0x4] |= timeMid >> 8;
	result._data[0x5] |= timeMid;

	const auto timeLow = time & 0x0fff;
	// version | timeHigh (in one byte)
	result._data[0x6] |= (6 << 4) | (timeLow >> 8);
	result._data[0x7] |= timeLow;

	uint16_t clockSeq;
	RAND_bytes((u8*)&clockSeq, sizeof(clockSeq));
	clockSeq &= 0x3fff; // retain right 14 bits
	// variant | 6 left bits of clockSeq
	result._data[0x8] |= 0b10000000 | (clockSeq >> 8);
	result._data[0x9] |= clockSeq;

	if (useMAC) {
		std::copy_n(_cachedMAC.data(), 6, &result._data[0xa]);
	} else {
		RAND_bytes(&result._data[0xa], 6);
	}

	return result;
}

UUID UUID::_fromName(std::string_view name, HashFn hashFn) noexcept {
	return _fromNamespacedName(_cachedInvalid, name, hashFn);
}

UUID UUID::_fromNamespacedName(const UUID& nameSpace, std::string_view name, HashFn hashFn) noexcept {
	UUID result;

	const auto catedDataSize = 16 + name.length();
	// RFC requires UUID bytes of nameSpace to be concatenated with bytes of name
	auto catedData = std::make_unique<u8[]>(catedDataSize);
	std::copy(nameSpace._data.data(), nameSpace._data.data() + 16, catedData.get());
	std::copy(name.data(), name.data() + name.size(), catedData.get() + 16);

	if (hashFn == md5) {
		EVP_MD_CTX* ctx = EVP_MD_CTX_new();
		if (!ctx) {
			return UUID();
		}

		if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1 ||
			EVP_DigestUpdate(ctx, catedData.get(), catedDataSize) != 1 ||
			EVP_DigestFinal_ex(ctx, result._data.data(), nullptr) != 1) {
			EVP_MD_CTX_free(ctx);
			return UUID();
		}
		EVP_MD_CTX_free(ctx);
	} else if (hashFn == sha1) {
		// SHA-1's results are 20 bytes, need to cut some of it
		std::array<u8, 20> temp{};
		SHA1((const u8*)catedData.get(), catedDataSize, temp.data());
		std::copy_n(temp.data(), result._data.size(), result._data.data());
	}

	result._data[0x6] &= 0x0f;
	// version:
	// 3 for MD5
	// 5 for SHA-1
	result._data[0x6] |= (hashFn == md5 ? 3 : 5) << 4;

	result._data[0x8] &= 0b00111111;
	// variant
	result._data[0x8] |= 0b10000000;

	return result;
}

UUID UUID::v3(std::string_view name) noexcept {
	return _fromName(name, md5);
}

UUID UUID::v3(const UUID& nameSpace, std::string_view name) noexcept {
	return _fromNamespacedName(nameSpace, name, md5);
}

UUID UUID::v5(std::string_view name) noexcept {
	return _fromName(name, sha1);
}

UUID UUID::v5(const UUID& nameSpace, std::string_view name) noexcept {
	return _fromNamespacedName(nameSpace, name, sha1);
}

UUID UUID::v4() noexcept {
	UUID result;

	RAND_bytes((u8*)result._data.data(), sizeof(UUID));

	result._data[0x6] &= 0x0f;
	// version
	result._data[0x6] |= 4 << 4;

	result._data[0x8] &= 0b00111111;
	// variant
	result._data[0x8] |= 0b10000000;

	return result;
}

UUID UUID::v7() noexcept {
	UUID result;

	uint64_t sinceEpoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
			.count();
	result._data[0x0] |= sinceEpoch >> 40;
	result._data[0x1] |= sinceEpoch >> 32;
	result._data[0x2] |= sinceEpoch >> 24;
	result._data[0x3] |= sinceEpoch >> 16;
	result._data[0x4] |= sinceEpoch >> 8;
	result._data[0x5] |= sinceEpoch;

	RAND_bytes(&result._data[0x6], 10);

	result._data[0x6] &= 0x0f;
	// version
	result._data[0x6] |= 7 << 4;

	result._data[0x8] &= 0b00111111;
	// variant
	result._data[0x8] |= 0b10000000;

	return result;
}

UUID UUID::v8(const std::array<u8, 16>& data) noexcept {
	UUID result;

	std::copy(data.data(), data.data() + 16, result._data.data());

	result._data[0x6] &= 0x0f;
	// version
	result._data[0x6] |= 8 << 4;

	result._data[0x8] &= 0b00111111;
	// variant
	result._data[0x8] |= 0b10000000;

	return result;
}

u32 UUID::version() const noexcept {
	return _data[0x6] >> 4;
}

u32 UUID::variant() const noexcept {
	return _data[0x8] >> 6;
}

bool UUID::invalid() const noexcept {
	return _data == _cachedInvalid._data;
}

UUID::operator bool() const noexcept {
	return invalid();
}

std::strong_ordering UUID::operator<=>(const UUID& other) const noexcept {
	return _data <=> other._data;
}

const std::array<u8, 16>& UUID::data() const noexcept {
	return _data;
}

std::string UUID::toString() const noexcept {
	std::string result;
	result.resize(37, ' ');

	std::sprintf(
		result.data(),
		"%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%"
		"02hhx",
		_data[0x0],
		_data[0x1],
		_data[0x2],
		_data[0x3],
		_data[0x4],
		_data[0x5],
		_data[0x6],
		_data[0x7],
		_data[0x8],
		_data[0x9],
		_data[0xa],
		_data[0xb],
		_data[0xc],
		_data[0xd],
		_data[0xe],
		_data[0xf]
	);
	result.pop_back();

	return result;
}

} // namespace arch::utils
