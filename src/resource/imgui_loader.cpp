#include "resource/imgui_loader.h"

namespace arch {

ImGuiLoader::ImGuiLoader(GLFWwindow* wnd) : _glfwWindow(wnd) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_glfwWindow, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

ImGuiLoader::~ImGuiLoader() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLoader::RenderNextF() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLoader::RenderLastF() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLoader::ShowStats() {
	ImGui::Begin("Window A");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
				1000.0 / (ImGui::GetIO().Framerate),
				(ImGui::GetIO().Framerate));
	ImGui::End();
}

}  // namespace arch
