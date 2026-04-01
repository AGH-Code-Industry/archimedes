#pragma once

#include <cstdint>
#include <format>
#include <iostream>

#include "EntityTraits.h"

/// @brief std::formatter specialization for Entity
template<>
struct std::formatter<arch::ecs::Entity> {
private:
	uint32_t _width = 0;
	char _type = 'd';
	char _versionSep = '\0';
	char _fill = ' ';
	uint8_t _flags = 0;

	enum Flags : uint8_t {
		hasIdx = 1 << 0,
		hasPrefix = 1 << 1,
		alignLeft = 1 << 2,
		alignCenter = 1 << 3,
		alignRight = 1 << 4,
		align = alignLeft | alignRight | alignCenter
	};

public:

	/// @brief Visitor operator
	/// @tparam T - value type to visit
	/// @param val - visited value
	/// @return value of val
	template<class T>
	constexpr uint32_t operator()(const T val) const;

	/// @brief Parses format specification
	/// @param ctx - parsing context
	constexpr std::format_parse_context::iterator parse(std::format_parse_context& ctx);

	/// @brief Formats entity
	/// @param entity - entity to format
	/// @param ctx - formatting context
	std::format_context::iterator format(const arch::ecs::Entity entity, std::format_context& ctx) const noexcept;
};

#include "EntityFormatter.hpp"
