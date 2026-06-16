#pragma once

namespace arch {

struct Application {
	virtual void init() {}
	virtual void update() {}
	virtual ~Application() {}
};

}
