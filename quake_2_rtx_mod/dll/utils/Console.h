#pragma once

#include <Windows.h>

#include <iostream>

class C_Console {
public:
	void init();
	void destroy();
	void printf(std::string_view format, ...);
private:
	HANDLE hHandle;
	char cCharBuffer[512];
};

inline C_Console console;