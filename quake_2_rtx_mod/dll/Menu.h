#pragma once

#include <Windows.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

class C_Menu {
public:
	void init(HWND windowHandle);
	void destroy();
	void onFrame();
public:
	bool autoHop = false;
	bool godMode = false;
	bool forceArmor = false;
};

inline C_Menu menu;