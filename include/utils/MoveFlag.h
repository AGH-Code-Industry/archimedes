#pragma once

namespace arch {

struct MoveFlag {
	explicit MoveFlag() = default;
};

static inline constexpr MoveFlag moveFlag;

} // namespace arch
