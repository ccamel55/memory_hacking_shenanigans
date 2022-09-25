#include "Hooks.h"
#include "Main.h"
#include "Menu.h"

#include "helpers/HookManager.h"

#include "utils/PatternScanner.h"
#include "utils/Console.h"
#include "utils/InputManager.h"

static bool loaded = false;
static bool openWindow = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

uint8_t* get_wglSwapBuffers()
{
	auto hMod = GetModuleHandleA("OPENGL32.dll");
	if (!hMod) return nullptr;

	return (uint8_t*)GetProcAddress(hMod, "wglSwapBuffers");
}

void C_Hooks::init() {

	hookManager.init();

	hookManager.hookFn(SV_LoadGameLibrary, globals.pFn_SV_LoadGameLibrary, &HK_SV_LoadGameLibrary);
	hookManager.hookFn(SwapBuffers, get_wglSwapBuffers(), &HK_SwapBuffers);

	O_WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(FindWindow(NULL, "Quake II RTX"), GWLP_WNDPROC, (LONG_PTR)HK_WndProc));
}

void C_Hooks::destroy() {
	hookManager.destroy();
	menu.destroy();
}

// wndproc - input processing
LRESULT __stdcall C_Hooks::HK_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	// all key bind shit here
	inputManager.onWndProcStart(uMsg, wParam, lParam);
	{
		if (inputManager.isKeyPressed(VK_HOME))
			openWindow = !openWindow;
	}
	inputManager.onWndProcEnd();

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) && openWindow)
		return true;

	return CallWindowProcW(hooks.O_WndProc, hWnd, uMsg, wParam, lParam);
}

void* __stdcall C_Hooks::HK_SV_LoadGameLibrary(const char* path) {

	// called when we load the game dll which contains all the logic and stuff we want to fuck with
	static const auto original = hookManager.getOriginal<FN_SV_LoadGameLibrary>(SV_LoadGameLibrary);
	
	const auto ret = original(path);

	if (globals.pGameLibraryAddress == NULL) {

		globals.pGameLibraryAddress = reinterpret_cast<uint8_t*>(GetModuleHandle("gamex86_64.dll"));
		console.printf("gamex86_64.dll address: 0x%llx", globals.pGameLibraryAddress);

		// hook game dll functions
		globals.pFn_ClientThink = patternScanner.addressFromPattern(globals.pGameLibraryAddress, "40 55 56 57");
		hookManager.hookFn(ClientThink, globals.pFn_ClientThink, &HK_ClientThink);
	}

	return ret;
}

void __stdcall C_Hooks::HK_ClientThink(edict_t* client, usercmd_t* cmd) {

	static const auto original = hookManager.getOriginal<FN_ClientThink>(ClientThink);
	
	// no damage / god mode
	if (menu.godMode)
		client->takedamage() = 0; // take damage is an int, 0 just means no damage

	// ensure power armour flag is always 1
	if (menu.forceArmor)
		client->flags() |= FL_POWER_ARMOR;

	// bunny hop
	if (menu.autoHop) {

		if (client->moveFlags() & PMF_JUMP_HELD) {

			if (!(client->moveFlags() & PMF_ON_GROUND)) {
				client->moveFlags() &= ~PMF_JUMP_HELD;
			}
		}
	}

	original(client, cmd);
}

int __stdcall C_Hooks::HK_SwapBuffers(IN HDC hDc)
{
	static const auto original = hookManager.getOriginal<FN_SwapBuffers>(SwapBuffers);

	if (!loaded) {
		menu.init(FindWindow(NULL, "Quake II RTX"));
		loaded = true;
	}

	// prepare for drawing 
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		if (openWindow)
			menu.onFrame();
	}

	// render everything in the draw list
	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return original(hDc);
}