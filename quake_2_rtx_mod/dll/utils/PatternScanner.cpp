#include "PatternScanner.h"

std::vector<int> C_PatternScanner::patternToByteArray(const std::string& pattern) {

	// ? in pattern means that we can have any value, we will replace ? with a -1 in the array

	std::vector<int> ret = {};
	
    const auto start = const_cast<char*>(pattern.data());
    const auto end = const_cast<char*>(pattern.data()) + pattern.length();
    
    for (char* i = start; i < end; i++) {

        if (*i == '?') {
            ret.push_back(-1);
            i++;
        }
        else {
            // convert string to a base16 integer
            ret.push_back(strtoul(i, &i, 16));
        }
    }

	return ret;
}

uint8_t* C_PatternScanner::addressFromPattern(uint8_t* moduleBaseAddress, const std::string& pattern) {

    // loop through the binary file and check if the bytes follow the sequence we specified in our pattern
    // we return the address of the first location that has a byte sequence that matches our pattern

    const auto patternArray = patternToByteArray(pattern);

    const auto patternSize = patternArray.size();
    const auto patternData = patternArray.data();

    const auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBaseAddress);
    const auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(moduleBaseAddress) + dosHeader->e_lfanew);

    // start at base address
    auto curByte = reinterpret_cast<uint8_t*>(moduleBaseAddress);

    for (uintptr_t i = 0; i < ntHeaders->OptionalHeader.SizeOfImage - patternSize; i++) {

        bool foundAddress = true;
           
        // check sequence of bytes
        for (uintptr_t j = 0; j < patternSize; j++) {
            
            // if byte matches the sequence or is -1 which means any byte is valid, then keep going else bye!!
            if (patternData[j] == -1 || curByte[i + j] == patternData[j])
                continue;

            foundAddress = false;
            break;
        }

        // oh look we got it yay!!
        if (foundAddress)
            return &curByte[i];
    }

    return NULL;
}