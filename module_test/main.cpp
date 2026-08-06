import arch;

namespace log = arch::log;
using arch::dupa;

int main() {
	log::init("Archimedes", false);

	dupa();
	log::debug("dupa {}", 0.5);
	dupa();
}
