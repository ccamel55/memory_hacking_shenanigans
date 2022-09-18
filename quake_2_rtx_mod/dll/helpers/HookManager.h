#pragma once

#include <Windows.h>
#include <MinHook.h>

#include <map>

struct S_HookedFnInfo {
	uint8_t* pAddress;
	uint8_t* pOriginal;
};

class C_HookManager {
public:
	void init();
	void destroy();
	bool hookFn(size_t id, uint8_t* address, LPVOID fn);
	bool unhookFn(size_t id);

	template<typename T>
	T getOriginal(size_t id) {
		return reinterpret_cast<T>(mHookedFn[id].pOriginal);
	}

private:
	std::map<size_t, S_HookedFnInfo> mHookedFn;
};

inline C_HookManager hookManager;