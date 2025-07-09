#include "TestApp.h"

#include <Engine.h>
#include <Logger.h>

int main() {
	arch::Logger::init(arch::LogLevel::trace);
	TestHashing();
	TestAssetManager();
	std::getchar();
}
