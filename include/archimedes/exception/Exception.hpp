#include <memory>

#include "Exception.h"

namespace arch {

template<class Self>
auto&& Exception::withStacktrace(this Self&& self, const std::stacktrace stacktrace) {
	if constexpr (buildinfo::Type::current == buildinfo::Type::Release) {
		log::warn("stacktraces are unavailable in Release mode");
	} else {
		self._hasStacktrace = true;

		std::destroy_at(std::addressof(self._location));
		std::construct_at(std::addressof(self._stacktrace), stacktrace);
	}

	return std::forward<Self>(self);
}

} // namespace arch
