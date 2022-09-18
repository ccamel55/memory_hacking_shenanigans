#pragma once

#include <Windows.h>
#include <stdint.h>

class C_Globals {
public:
	void memoryInit();
public:
	uint8_t* pExeBaseAddress;
	uint8_t* pFn_R_EndFrame_GL;
};

inline C_Globals globals;