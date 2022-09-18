#pragma once

#include <Windows.h>
#include <stdint.h>

class C_Hooks {
public:
	void init();
	void destroy();
private:
	enum E_HookedFn : size_t {
		R_EndFrame_GL,
	};

	WNDPROC O_WndProc;
	static LRESULT __stdcall HK_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	using FN_R_EndFrame_GL = void(__stdcall*)();
	static void __stdcall HK_R_EndFrame_GL();
};

inline C_Hooks hooks;