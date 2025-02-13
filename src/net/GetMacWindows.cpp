#include <algorithm>

#include <net/GetMac.h>

#if ARCHIMEDES_WINDOWS

bool arch::net::getMac(std::array<arch::u8, 6>& result) noexcept {
	IP_ADAPTER_INFO adaptersInfo[16]; // Allocate information for up to 16 adapters
	DWORD bufLen = sizeof(adaptersInfo);
	if (GetAdaptersInfo(adaptersInfo, &bufLen) != ERROR_SUCCESS) {
		result = {};
		return false;
	}

	IP_ADAPTER_INFO* adaptersInfoPtr = adaptersInfo;
	while (adaptersInfoPtr) {
		if (adaptersInfoPtr->AddressLength == 6) {
			std::copy_n(adaptersInfoPtr->Address, 6, result.begin());
			return true;
		}
		adaptersInfoPtr = adaptersInfoPtr->Next;
	}

	result = {};
	return false;
}

#endif
