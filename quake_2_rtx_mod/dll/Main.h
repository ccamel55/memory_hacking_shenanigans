#pragma once

#include <Windows.h>
#include <stdint.h>

class C_Globals {
public:
	void memoryInit();
public:
	uint8_t* pExeBaseAddress;
	uint8_t* pGameLibraryAddress;

	uint8_t* pFn_SV_LoadGameLibrary;

	uint8_t* pFn_ClientThink;
};

inline C_Globals globals;