#include "examples/Examples.h"

#include <Logger.h>

int main() {

	arch::Logger::init(arch::LogLevel::trace);
	TestHashing();
	TestImporting();
}