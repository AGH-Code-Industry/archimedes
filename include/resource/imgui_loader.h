#pragma once

#include "../bindings/imgui_impl_glfw.h"
#include "../bindings/imgui_impl_opengl3.h"

namespace arch {

class ImGuiLoader {
public:
	ImGuiLoader(GLFWwindow* wnd);
	~ImGuiLoader();

	void RenderNextF();
	void RenderLastF();

	void ShowStats();

private:
	GLFWwindow* _glfwWindow;
};

}  // namespace arch
