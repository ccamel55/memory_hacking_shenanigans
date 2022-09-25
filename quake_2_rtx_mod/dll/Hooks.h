#pragma once

#include <Windows.h>
#include <stdint.h>

#include "sdk/structs.h"

class C_Hooks {
public:
	void init();
	void destroy();
private:
	enum E_HookedFn : size_t {
		SV_LoadGameLibrary,
		ClientThink,
		SwapBuffers,
	};

	WNDPROC O_WndProc;
	static LRESULT __stdcall HK_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	using FN_SV_LoadGameLibrary = void* (__stdcall*)(const char*);
	static void* __stdcall HK_SV_LoadGameLibrary(const char* path);

	using FN_ClientThink = void(__stdcall*)(edict_t*, usercmd_t*);
	static void __stdcall HK_ClientThink(edict_t* client, usercmd_t* cmd);

	using FN_SwapBuffers = int(__stdcall*) (HDC hDc);
	static int __stdcall HK_SwapBuffers(IN HDC hDc);
};

inline C_Hooks hooks;