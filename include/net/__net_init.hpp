#pragma once

namespace arch::net {
	class __NetworkAuto final {
	public:
		bool initialized = false;
		__NetworkAuto();
		~__NetworkAuto();
	};

	const __NetworkAuto __net_auto; // will create at the begining and destroy at the end
}