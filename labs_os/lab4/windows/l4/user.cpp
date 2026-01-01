#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <tlhelp32.h>
#include <chrono>
#include <thread>

bool IsProcessRunning(const std::wstring& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);
    
    if (Process32FirstW(hSnapshot, &pe)) {
        do {
            std::wstring name = pe.szExeFile;
            if (_wcsicmp(name.c_str(), processName.c_str()) == 0 ||
                _wcsicmp(name.c_str(), (processName + L".exe").c_str()) == 0) {
                CloseHandle(hSnapshot);
                return true;
            }
        } while (Process32NextW(hSnapshot, &pe));
    }
    
    CloseHandle(hSnapshot);
    return false;
}

DWORD GetProcessIdByName(const std::wstring& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) 
        return 0;
    
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);
    
    if (Process32FirstW(hSnapshot, &pe)) {
        do {
            std::wstring name = pe.szExeFile;
            if (_wcsicmp(name.c_str(), processName.c_str()) == 0 ||
                _wcsicmp(name.c_str(), (processName + L".exe").c_str()) == 0) {
                DWORD pid = pe.th32ProcessID;
                CloseHandle(hSnapshot);
                return pid;
            }
        } while (Process32NextW(hSnapshot, &pe));
    }
    
    CloseHandle(hSnapshot);
    return 0;
}

int main() {
    std::string processesToKill = "notepad,notepad.exe,NOTEPAD";
    std::cout << "Setting environment variable PROC_TO_KILL = " << processesToKill << std::endl;
    
    if (!SetEnvironmentVariableA("PROC_TO_KILL", processesToKill.c_str())) {
        std::cout << "Failed to set environment variable!" << std::endl;
        return 1;
    }

    std::cout << "\nStarting test processes (notepad only)..." << std::endl;
    
    system("start notepad.exe");
    std::cout << "Started: Notepad #1" << std::endl;
    
    Sleep(500);
    system("start notepad.exe");
    std::cout << "Started: Notepad #2" << std::endl;
    
    Sleep(500);
    system("start notepad.exe");
    std::cout << "Started: Notepad #3" << std::endl;
    
    Sleep(2000);
    
    std::cout << "\nChecking if notepad processes are running..." << std::endl;
    
    bool notepadRunning = IsProcessRunning(L"notepad");
    std::cout << "Notepad processes are running: " << (notepadRunning ? "YES" : "NO") << std::endl;
    
    int notepadCount = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(PROCESSENTRY32W);
        
        if (Process32FirstW(hSnapshot, &pe)) {
            do {
                std::wstring name = pe.szExeFile;
                if (_wcsicmp(name.c_str(), L"notepad.exe") == 0) {
                    notepadCount++;
                    std::wcout << L"  Found: " << name << L" (PID: " << pe.th32ProcessID << L")" << std::endl;
                }
            } while (Process32NextW(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }
    
    std::cout << "Total notepad processes: " << notepadCount << std::endl;
    
    std::cout << "\n    Test 1: Killing by name    " << std::endl;
    std::cout << "Running: Killer.exe --name notepad" << std::endl;
    system("killer.exe --name notepad");
    
    Sleep(1000);
    
    int notepadCountAfter = 0;
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(PROCESSENTRY32W);
        
        if (Process32FirstW(hSnapshot, &pe)) {
            do {
                std::wstring name = pe.szExeFile;
                if (_wcsicmp(name.c_str(), L"notepad.exe") == 0) {
                    notepadCountAfter++;
                }
            } while (Process32NextW(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }
    
    std::cout << "Notepad processes after killing by name: " << notepadCountAfter << std::endl;
    
    std::cout << "\nStarting more notepad processes for next test..." << std::endl;
    system("start notepad.exe");
    system("start notepad.exe");
    Sleep(1000);
    
    std::cout << "\n   Test 2: Killing from environment variable " << std::endl;
    
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    int beforeCount = 0;
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(PROCESSENTRY32W);
        
        if (Process32FirstW(hSnapshot, &pe)) {
            do {
                std::wstring name = pe.szExeFile;
                if (_wcsicmp(name.c_str(), L"notepad.exe") == 0) {
                    beforeCount++;
                }
            } while (Process32NextW(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }
    
    std::cout << "Notepad processes before: " << beforeCount << std::endl;
    std::cout << "Running: killer.exe (without parameters)" << std::endl;
    system("killer.exe");
    
    Sleep(1000);
    
    std::cout << "\nFinal status:" << std::endl;
    bool finalStatus = IsProcessRunning(L"notepad");
    std::cout << "Notepad processes are running: " << (finalStatus ? "YES" : "NO") << std::endl;
    
    std::cout << "\nCleaning up environment variable..." << std::endl;
    if (SetEnvironmentVariableA("PROC_TO_KILL", NULL))
        std::cout << "Environment variable PROC_TO_KILL removed successfully" << std::endl;
    else 
        std::cout << "Failed to remove environment variable!" << std::endl;
    
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}