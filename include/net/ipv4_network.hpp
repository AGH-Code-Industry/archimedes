#pragma once

#include <net/ipv4.hpp>
#include <bit>

namespace arch::net {
	/// @brief Represents network mask of IPv4.
	///
	class IPv4Mask : public IPv4 {
	public:
		/// @brief Invalid mask (255.255.255.255).
		///
		static IPv4Mask invalid;

		data_type data() const;
		void data(data_type new_data) = delete;
		void data(binary_type new_data) = delete;
		void data(size_t octet, octet_type new_octet) = delete;

		/// @brief Default constructor, sets to invalid.
		///
		IPv4Mask();
		/// @brief Decimal constructor.
		/// @param decimal - decimal form of mask.
		IPv4Mask(std::string_view decimal);
		/// @brief Prefix constructor.
		/// @param prefix - prefix of mask.
		IPv4Mask(size_t prefix);
	};

	/// @brief Represents IPv4 network.
	///
	class IPv4Network {
	public:
		/// @brief Alias of IPv4.
		///
		using address_type = IPv4;
		/// @brief Storage type for masks.
		///
		using mask_type = IPv4Mask;

	private:
		address_type _address;
		mask_type _mask;

	public:
		/// @brief Decimal constructor.
		/// @param address - network address.
		/// @param mask - mask.
		IPv4Network(address_type address, mask_type mask);
		/// @brief Prefix constructor.
		/// @param address - network address. 
		/// @param prefix - mask prefix.
		IPv4Network(address_type address, size_t prefix);
		/// @brief Mixed constructor.
		/// @param address_with_prefix - decimal form of address with prefix (xxx.xxx.xxx.xxx/xx).
		IPv4Network(std::string_view address_with_prefix);

		/// @brief Broadcast address of network.
		/// 
		address_type broadcast() const;
		/// @brief If network contains given address.
		/// @param address - address to check.
		bool contains(address_type address) const;
		/// @brief Address of first host in network.
		///
		address_type first_host() const;
		/// @brief Address of last host in network.
		///
		address_type last_host() const;
		/// @brief Address of this network.
		///
		address_type network_address() const;
		/// @brief Mask of this network.
		///
		mask_type mask() const;
	};

	constexpr bool __i_use_arch_btw = false;
}