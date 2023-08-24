#pragma once

#include "exception.h"

namespace arch {

class GladException : public Exception {
public:
	GladException();
	GladException(const std ::string description);
};

}
