#include <memory>

#include "Exception.h"

namespace arch {

template<class Self>
auto&& Exception::withStacktrace(this Self&& self, std::stacktrace stacktrace) {
	if constexpr (buildinfo::Type::current == buildinfo::Type::Release) {
		log::warn("stacktraces are unavailable in Release mode");
	} else {
		self._locationOrStacktrace = std::move(stacktrace);
	}

	return std::forward<Self>(self);
}

} // namespace arch
