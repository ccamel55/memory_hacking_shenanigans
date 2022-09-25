#pragma once

#include <Windows.h>
#include <windowsx.h>

#include <array>

class C_InputManager {
public:
	void onWndProcStart(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void onWndProcEnd();
	bool isKeyDown(UINT vkCode);
	bool isKeyPressed(UINT vkCode);
	bool isKeyReleased(UINT vkCode);
private:
	UINT uCurKey = 0u;
	bool bCurState = false;

	int iScrollState = 0;
	POINT pMousePos = {};

	std::array<bool, 256u> aKeyState = {};
	std::array<bool, 256u> aPrevKeyState = {};
};

inline C_InputManager inputManager;
