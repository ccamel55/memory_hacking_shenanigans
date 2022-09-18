#pragma once

#include <Windows.h>

#include <string>
#include <vector>

class C_PatternScanner {
public:
	std::vector<int> patternToByteArray(const std::string& pattern);
	uint8_t* addressFromPattern(uint8_t* moduleBaseAddress, const std::string& pattern);
};

inline C_PatternScanner patternScanner;