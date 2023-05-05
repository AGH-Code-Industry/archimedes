#include <engine.h>
#include <spdlog/spdlog.h>

int main() {
	spdlog::set_level(spdlog::level::debug);

	arch::Engine engine {};
	engine.start();
}
