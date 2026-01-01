#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <tlhelp32.h>

BOOL KillProcessById(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
    if (hProcess == NULL) {
        std::cout << "Failed to open process with ID: " << processId << std::endl;
        return FALSE;
    }
    
    BOOL result = TerminateProcess(hProcess, 0);
    if (result) 
        std::cout << "Successfully terminated process with ID: " << processId << std::endl;
    else 
        std::cout << "Failed to terminate process with ID: " << processId << std::endl;
    
    CloseHandle(hProcess);
    return result;
}

BOOL KillProcessesByName(const std::wstring& processName) {
    bool found = false;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create process snapshot" << std::endl;
        return FALSE;
    }
    
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);
    
    if (Process32FirstW(hSnapshot, &pe)) {
        do {
            std::wstring name = pe.szExeFile;
            
            if (_wcsicmp(name.c_str(), processName.c_str()) == 0 ||
                _wcsicmp(name.c_str(), (processName + L".exe").c_str()) == 0) {
                
                std::wcout << L"Found process: " << name << L" (PID: " << pe.th32ProcessID << L")" << std::endl;
                
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProcess != NULL) {
                    if (TerminateProcess(hProcess, 0)) {
                        std::wcout << L"Successfully terminated: " << name << std::endl;
                        found = true;
                    } else std::cout << "Failed to terminate process" << std::endl;
                    CloseHandle(hProcess);
                }
            }
        } while (Process32NextW(hSnapshot, &pe));
    }
    
    CloseHandle(hSnapshot);
    
    if (!found) 
        std::wcout << L"No processes found with name: " << processName << std::endl;

    return found;
}

std::vector<std::wstring> GetProcessesFromEnvironment() {
    std::vector<std::wstring> processes;
    char buffer[4096];
    
    DWORD size = GetEnvironmentVariableA("PROC_TO_KILL", buffer, sizeof(buffer));
    if (size > 0 && size < sizeof(buffer)) {
        std::string envValue = buffer;
        std::stringstream ss(envValue);
        std::string processName;
        
        while (std::getline(ss, processName, ',')) {
            size_t start = processName.find_first_not_of(" \t");
            size_t end = processName.find_last_not_of(" \t");
            if (start != std::string::npos && end != std::string::npos) {
                processName = processName.substr(start, end - start + 1);
                
                if (!processName.empty()) {
                    int wsize = MultiByteToWideChar(CP_UTF8, 0, processName.c_str(), -1, NULL, 0);
                    wchar_t* wstr = new wchar_t[wsize];
                    MultiByteToWideChar(CP_UTF8, 0, processName.c_str(), -1, wstr, wsize);
                    processes.push_back(wstr);
                    delete[] wstr;
                }
            }
        }
    }
    
    return processes;
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--id" && i + 1 < argc) {
            DWORD processId = std::stoi(argv[++i]);
            std::cout << "Killing process by ID: " << processId << std::endl;
            KillProcessById(processId);
        }
        else if (arg == "--name" && i + 1 < argc) {
            std::string processName = argv[++i];
            
            int wsize = MultiByteToWideChar(CP_UTF8, 0, processName.c_str(), -1, NULL, 0);
            wchar_t* wstr = new wchar_t[wsize];
            MultiByteToWideChar(CP_UTF8, 0, processName.c_str(), -1, wstr, wsize);
            std::wstring wprocessName = wstr;
            delete[] wstr;
            
            std::cout << "Killing processes by name: " << processName << std::endl;
            KillProcessesByName(wprocessName);
        }
        else if (arg == "--help") {
            std::cout << "Usage:" << std::endl;
            std::cout << "  killer.exe --id <process_id>" << std::endl;
            std::cout << "  killer.exe --name <process_name>" << std::endl;
            std::cout << "  killer.exe (will kill processes from PROC_TO_KILL env var)" << std::endl;
        }
    }
    
    if (argc == 1) {
        std::cout << "Checking PROC_TO_KILL environment variable..." << std::endl;
        auto processes = GetProcessesFromEnvironment();
        
        if (!processes.empty()) {
            std::cout << "Found processes to kill from environment variable:" << std::endl;
            for (const auto& proc : processes) 
                std::wcout << L"  - " << proc << std::endl;
            
            std::cout << "\nKilling notepad processes only..." << std::endl;
            for (const auto& proc : processes) 
                if (_wcsicmp(proc.c_str(), L"notepad") == 0 || 
                    _wcsicmp(proc.c_str(), L"notepad.exe") == 0) {
                    KillProcessesByName(proc);
                }
        } else std::cout << "No processes specified in PROC_TO_KILL environment variable" << std::endl;
    }
    
    return 0;
}