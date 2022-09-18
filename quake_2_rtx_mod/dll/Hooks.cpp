#include "Hooks.h"
#include "Main.h"
#include "helpers/HookManager.h"

void C_Hooks::init() {

	hookManager.init();
	hookManager.hookFn(R_EndFrame_GL, globals.pFn_R_EndFrame_GL, &HK_R_EndFrame_GL);
	
	O_WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(FindWindow(NULL, "Quake II RTX"), GWLP_WNDPROC, (LONG_PTR)HK_WndProc));
}

void C_Hooks::destroy() {
	hookManager.destroy();
}

// wndproc - input processing
LRESULT __stdcall C_Hooks::HK_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return CallWindowProcW(hooks.O_WndProc, hWnd, uMsg, wParam, lParam);
}

// R_EndFrame_GL - drawing opengl stuff
void __stdcall C_Hooks::HK_R_EndFrame_GL() {

	static const auto original = hookManager.getOriginal<FN_R_EndFrame_GL>(R_EndFrame_GL);
	

	original();
}
