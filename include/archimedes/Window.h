#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <archimedes/Math.h>
#include <archimedes/Monitor.h>
#include <archimedes/gfx/Buffer.h>

namespace arch {

class Window {
public:

	/// @brief Window mode
	enum Mode {
		windowed,
		fullscreen
	};

	Window(const Window& other) = delete;
	/**
	 * Constructor.
	 * @param width Window's width.
	 * @param height Window's height.
	 * @param name Window's name.
	 * @param monitor The pointer to the monitor to use for fullscreen mode.
	 * @param share The window whose context to share resources with, or NULL to not share resources.
	 */
	Window(int width, int height, const std::string& name, const Window& share);
	/**
	 * Constructor.
	 * @param width Window's width.
	 * @param height Window's height.
	 * @param name Window's name.
	 * @param monitor The monitor to use for fullscreen mode.
	 */
	Window(int width, int height, const std::string& name);

	/**
	 * @return Window's object.
	 */
	GLFWwindow* get() const;

	void swapBuffers() const;

	/// @brief Resizes window
	/// @param width - new width
	/// @param heigh - new height
	void resize(int width, int height);

	/// @brief Sets window title
	void setTitle(const std::string& title);

	/// @brief Sets fullscreen mode
	void setFullscreen();

	/// @brief Sets fullscreen mode
	/// @param monitor - monitor to set fullscreen on
	void setFullscreen(Monitor& monitor);

	/// @brief Sets windowed mode
	void setWindowed();

	/// @brief Toggles fullscreen
	void toggleFullscreen();
	/// @brief Toggles fullscreen
	/// @param monitor - monitor to set fullscreen on
	void toggleFullscreen(Monitor& monitor);

	/// @brief Returns window size
	int2 size() const;
	/// @brief Returns window mode
	Mode mode() const;

	bool shouldClose() const;

	Window& operator=(const Window&) = delete;

private:
	std::string _title;
	GLFWwindow* _window{};
	int2 _size{};
	int2 _sizeWindowed{};
	Mode _mode = windowed;

	/**
	 * Initializes window
	 * @param width Window's width.
	 * @param height Window's height.
	 * @param name Window's name.
	 * @param window The window whose context to share resources with, or NULL to not share resources.
	 */
	void _initialize(int width, int height, const char* name, GLFWwindow* window);
};

} // namespace arch
