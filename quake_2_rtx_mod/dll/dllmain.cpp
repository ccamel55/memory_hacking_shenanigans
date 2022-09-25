#include <Windows.h>

#include <chrono>
#include <thread>

#include "Main.h"
#include "Hooks.h"
#include "utils/Console.h"
#include "utils/PatternScanner.h"

void C_Globals::memoryInit() {

	// base address of exe can be found by passing NULL
	pExeBaseAddress = reinterpret_cast<uint8_t*>(GetModuleHandle(NULL));
	pGameLibraryAddress = NULL;

	pFn_SV_LoadGameLibrary = patternScanner.addressFromPattern(pExeBaseAddress, "48 89 5C 24 ? 57 48 83 EC 20 4C 8D 05 ? ? ? ?");
}

DWORD WINAPI dllThread(void* param)
{
	globals.memoryInit();

	console.init();
	console.printf("q2rtx.exe base address: 0x%llx", globals.pExeBaseAddress);

	hooks.init();

	while (true)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	hooks.destroy();
	console.destroy();

	// unload our dll from the process
	FreeLibraryAndExitThread(static_cast<HMODULE>(param), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) { 

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		const auto newThread = CreateThread(NULL, NULL, dllThread, hModule, NULL, NULL);

		if (newThread)
			CloseHandle(newThread);

		break;
	}
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}

    return TRUE;
}

