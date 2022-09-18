#include "HookManager.h"

void C_HookManager::init() {
	MH_Initialize();
}

void C_HookManager::destroy() {

	for (const auto& fn : mHookedFn)
		MH_DisableHook(fn.second.pAddress);

	mHookedFn.clear();
	MH_Uninitialize();
}

bool C_HookManager::hookFn(size_t id, uint8_t* address, LPVOID fn) {

	auto& curFn = mHookedFn[id];
	curFn.pAddress = address;
	
	if (MH_CreateHook(curFn.pAddress, fn, reinterpret_cast<LPVOID*>(&curFn.pOriginal)) == MH_OK)
		MH_EnableHook(curFn.pAddress);

	return true;
}

bool C_HookManager::unhookFn(size_t id) {

	const auto address = mHookedFn[id].pAddress;

	if (address)
		MH_DisableHook(address);

	return true;
}