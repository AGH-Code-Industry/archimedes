#pragma once

#include <GLFW/glfw3.h>
#include <archimedes/Math.h>
#include <archimedes/utils/OptRef.h>

namespace arch {

/// @brief Class of monitor
class Monitor {
public:
	/// @brief Returns ith monitor
	/// @param i (default = 0) - monitor number to obtain
	static utils::OptRef<Monitor> get(u32 i = 0);

	/// @brief Initializes monitor
	/// @param monitor - glfw monitor
	Monitor(GLFWmonitor* monitor);

	/// @brief Returns current monitor size
	/// @details Result is affected by window settings in fullscreen mode
	int2 currentSize() const;
	/// @brief Returns original monitor size
	int2 originalSize() const;

	/// @brief Returns current refresh rate
	/// @details Result is affected by window settings in fullscreen mode
	int currentRefreshRate() const;
	/// @brief Returns original monitor refresh rate
	int originalRefreshRate() const;

private:
	friend class Window;

	static std::vector<Monitor> _monitors;
	static std::vector<Monitor> _initMonitors();

	GLFWmonitor* _monitor;
	int2 _originalSize = {};
	int _originalRefreshRate = {};
};

} // namespace arch
