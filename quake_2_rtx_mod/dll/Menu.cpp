#include "Menu.h"

void C_Menu::init(HWND windowHandle) {

	const char* glsl_version = "#version 130";

	// init imgui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// wndproc hook and init opengl3
	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void C_Menu::destroy() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void C_Menu::onFrame() {

	ImGui::Begin("Hello, world!");                       
	{
		ImGui::Checkbox("God mode", &godMode);
		ImGui::Checkbox("Force armor", &forceArmor);
		ImGui::Checkbox("Auto Hop", &autoHop);
	}
	ImGui::End();
}