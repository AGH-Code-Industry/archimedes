#pragma once

namespace arch::utils {

template<class E>
class Err {
public:
	Err() noexcept = default;
	Err(const Err&) noexcept = default;
	Err(Err&&) noexcept = default;
	Err(const E& error) noexcept;

	Err& operator=(const Err&) noexcept = default;
	Err& operator=(Err&&) noexcept = default;
	Err& operator=(const E& error) noexcept;

private:

	template<class T, class E2>
	friend class Res;
	template<class T, class E2>
	friend class MoveRes;
	template<class T, class E2>
	friend class RefRes;

	E _error = {};
};

} // namespace arch::utils

#include "Err.hpp"

namespace arch {

using utils::Err;

}
