#pragma once

#include "Siblings.h"
#include <ecs/EntityTraits.h>

namespace arch::hier {

struct ChildNode: public Siblings {
	ecs::Entity entity = ecs::nullEntity;
};

} // namespace arch::hier
