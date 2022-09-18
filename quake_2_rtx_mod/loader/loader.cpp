#include <Windows.h>
#include <tlhelp32.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <filesystem>

/*
    forces a program to load a DLL file, this can work with any 64 bit process
    note: might not work with programs that have anti cheat
*/

void sleep(int sleepMs) {
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
}

DWORD findProcessID(const std::string& exeName) {

    // credits: https://cplusplus.com/forum/windows/12137/

    PROCESSENTRY32 processInfo = {};
    processInfo.dwSize = sizeof(processInfo);

    const auto processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    for (Process32First(processesSnapshot, &processInfo); Process32Next(processesSnapshot, &processInfo);) {

        // find first process that has the same exe name as the one we specified 
        if (exeName.compare(processInfo.szExeFile) != 0)
            continue;

        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    CloseHandle(processesSnapshot);
    return 0;
}

void injectDll(DWORD pid, const std::string& dllPath) {

    /*
        most simple dll injection method, we use CreateRemoteThread to create a thread in the target process which then will load our DLL into that process
    */
       
    if (pid == 0 || dllPath.empty())
        return;

    constexpr auto PROCESS_ACCESS = (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ);
    const auto targetProcess = OpenProcess(PROCESS_ACCESS, FALSE, pid);

    if (!targetProcess)
        return;

    const auto kernalModule = GetModuleHandle("kernel32.dll");
    
    if (!kernalModule)
        return;

    // make space for the dll path and allocate it to our process
    const auto dllPathStr = VirtualAllocEx(targetProcess, NULL, dllPath.length(), MEM_COMMIT, PAGE_READWRITE);

    if (!dllPathStr) {
        CloseHandle(targetProcess);
        return;
    }
    
    if (!WriteProcessMemory(targetProcess, dllPathStr, dllPath.c_str(), dllPath.length(), NULL)) {
        CloseHandle(targetProcess);
        return;
    }

    const auto loadLibraryFn = GetProcAddress(kernalModule, "LoadLibraryA");
    const auto remoteThread = CreateRemoteThread(targetProcess, NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryFn), dllPathStr, NULL, NULL);

    if (!remoteThread) {
        CloseHandle(targetProcess);
        return;
    }

    WaitForSingleObject(remoteThread, INFINITE);

    CloseHandle(remoteThread);
    CloseHandle(targetProcess);
}

int main(int argc, char* argv[]) {
       
    // we can launch with a specified process and dll to load or just open the console
    if (argc != 1 && argc != 3) {
        std::cout << "launching with arguments: [process.exe] [dll.dll]\n";

        sleep(1000);
        return 0;
    }

    DWORD procesID = 0;

    std::string targetProcess = "";
    std::string targetDll = "";

    // try get information about the process and dll 
    if (argc == 3) {

        // argv[0] is what what used to launch the .exe
        targetProcess = argv[1];
        targetDll = argv[2];

        procesID = findProcessID(targetProcess);
    }
    else {

        // let user enter process name and dll name
        targetProcess = "q2rtx.exe";
        targetDll = "dll.dll";

        procesID = findProcessID(targetProcess);
    }

    std::filesystem::path dllLocalPath = targetDll;
    const auto dllPath = std::filesystem::absolute(dllLocalPath);

    std::cout << "Target process: " << targetProcess.c_str() << " pid: " << procesID << "\n";
    std::cout << "Target dll: " << dllPath.string() << "\n";

    injectDll(procesID, dllPath.string());

    sleep(500);
    return 0;
}
