#include <archimedes/Monitor.h>
#include <archimedes/exceptions/GLFWException.h>

namespace arch {

std::vector<Monitor> Monitor::_initMonitors() {
	glfwInit();

	std::vector<Monitor> result;

	int monitorCount = 0;
	auto monitors = glfwGetMonitors(&monitorCount);
	if (monitorCount == 0) {
		throw arch::GLFWException();
	}

	result.reserve(monitorCount);
	for (int i = 0; i != monitorCount; ++i) {
		result.emplace_back(monitors[i]);
	}

	return result;
}

std::vector<Monitor> Monitor::_monitors = Monitor::_initMonitors();

utils::OptRef<Monitor> Monitor::get(u32 i) {
	if (i < _monitors.size()) {
		return _monitors[i];
	}
	return std::nullopt;
}

Monitor::Monitor(GLFWmonitor* monitor): _monitor{ monitor } {
	auto videoMode = glfwGetVideoMode(monitor);
	if (!videoMode) {
		throw GLFWException();
	}
	_originalSize = { videoMode->width, videoMode->height };
	_originalRefreshRate = videoMode->refreshRate;
}

int2 Monitor::currentSize() const {
	auto videoMode = glfwGetVideoMode(_monitor);
	if (!videoMode) {
		throw GLFWException();
	}
	return { videoMode->width, videoMode->height };
}

int2 Monitor::originalSize() const {
	return _originalSize;
}

int Monitor::currentRefreshRate() const {
	auto videoMode = glfwGetVideoMode(_monitor);
	if (!videoMode) {
		throw GLFWException();
	}
	return videoMode->refreshRate;
}

int Monitor::originalRefreshRate() const {
	return _originalRefreshRate;
}

} // namespace arch
