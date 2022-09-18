#include "Console.h"

void C_Console::init() {

	if (!AllocConsole())
		return;

	freopen_s(reinterpret_cast<_iobuf**>(stdin), "CONIN$", "r", stdin);
	freopen_s(reinterpret_cast<_iobuf**>(stdout), "CONOUT$", "w", stdout);
	freopen_s(reinterpret_cast<_iobuf**>(stderr), "CONOUT$", "w", stderr);

	hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitleA("Debug Console");
}

void C_Console::destroy() {

	fclose(static_cast<_iobuf*>(stdin));
	fclose(static_cast<_iobuf*>(stdout));
	fclose(static_cast<_iobuf*>(stderr));

	FreeConsole();
}

void C_Console::printf(std::string_view format, ...) {

	if (sizeof(format.data()) >= sizeof(cCharBuffer))
		return;

	va_list arguments = {};
	va_start(arguments, format);

	std::memset(cCharBuffer, '\0', sizeof(cCharBuffer));
	vsprintf_s(cCharBuffer, format.data(), arguments);

	SetConsoleTextAttribute(hHandle, 7);
	std::cout << cCharBuffer << std::endl;

	va_end(arguments);
}