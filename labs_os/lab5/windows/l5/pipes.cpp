#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>

#define BUFFER_SIZE 4096

void ProcessM(HANDLE hInput, HANDLE hOutput) {
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    std::stringstream ss;
    
    while (ReadFile(hInput, buffer, BUFFER_SIZE - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        ss << buffer;
        
        std::string numStr;
        while (std::getline(ss, numStr, ' ')) {
            if (!numStr.empty() && numStr != "\n") {
                try {
                    int x = std::stoi(numStr);
                    int result = x * 7;
                    std::string output = std::to_string(result) + " ";
                    WriteFile(hOutput, output.c_str(), output.size(), NULL, NULL);
                }
                catch (...) {
                  
                }
            }
        }
        
        ss.clear();
    }
    
    std::string eol = "\n";
    WriteFile(hOutput, eol.c_str(), eol.size(), NULL, NULL);
    CloseHandle(hOutput);
}

void ProcessA(HANDLE hInput, HANDLE hOutput) {
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    std::stringstream ss;
    
    while (ReadFile(hInput, buffer, BUFFER_SIZE - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        ss << buffer;
        
        std::string numStr;
        while (std::getline(ss, numStr, ' ')) {
            if (!numStr.empty() && numStr != "\n") {
                try {
                    int x = std::stoi(numStr);
                    int result = x + 14;
                    std::string output = std::to_string(result) + " ";
                    WriteFile(hOutput, output.c_str(), output.size(), NULL, NULL);
                }
                catch (...) {
                    
                }
            }
        }
        
        ss.clear();
    }
    
    std::string eol = "\n";
    WriteFile(hOutput, eol.c_str(), eol.size(), NULL, NULL);
    CloseHandle(hOutput);
}

void ProcessP(HANDLE hInput, HANDLE hOutput) {
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    std::stringstream ss;
    
    while (ReadFile(hInput, buffer, BUFFER_SIZE - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        ss << buffer;
        
        std::string numStr;
        while (std::getline(ss, numStr, ' ')) {
            if (!numStr.empty() && numStr != "\n") {
                try {
                    int x = std::stoi(numStr);
                    int result = x * x * x; 
                    std::string output = std::to_string(result) + " ";
                    WriteFile(hOutput, output.c_str(), output.size(), NULL, NULL);
                }
                catch (...) {
                }
            }
        }
        
        ss.clear();
    }

    std::string eol = "\n";
    WriteFile(hOutput, eol.c_str(), eol.size(), NULL, NULL);
    CloseHandle(hOutput);
}

void ProcessS(HANDLE hInput, HANDLE hOutput) {
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    std::stringstream ss;
    int sum = 0;
    
    while (ReadFile(hInput, buffer, BUFFER_SIZE - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        ss << buffer;
        
        std::string numStr;
        while (std::getline(ss, numStr, ' ')) {
            if (!numStr.empty() && numStr != "\n") {
                try {
                    int x = std::stoi(numStr);
                    sum += x;
                }
                catch (...) {
                }
            }
        }
        
        ss.clear();
    }
    
    std::string result = "Sum: " + std::to_string(sum) + "\n";
    WriteFile(hOutput, result.c_str(), result.size(), NULL, NULL);
    CloseHandle(hOutput);
}

bool CreateChildProcess(const char* processName, HANDLE hInput, HANDLE hOutput, 
                       PROCESS_INFORMATION& pi, void (*processFunc)(HANDLE, HANDLE)) {
    std::thread thread(processFunc, hInput, hOutput);
    thread.detach();
    
    return true;
}

int main() {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    
    HANDLE hPipeM_A_Read, hPipeM_A_Write;  // M -> A
    HANDLE hPipeA_P_Read, hPipeA_P_Write;  // A -> P
    HANDLE hPipeP_S_Read, hPipeP_S_Write;  // P -> S
    HANDLE hPipeS_Out_Read, hPipeS_Out_Write; // S -> Output
    
    CreatePipe(&hPipeM_A_Read, &hPipeM_A_Write, &sa, 0);
    CreatePipe(&hPipeA_P_Read, &hPipeA_P_Write, &sa, 0);
    CreatePipe(&hPipeP_S_Read, &hPipeP_S_Write, &sa, 0);
    CreatePipe(&hPipeS_Out_Read, &hPipeS_Out_Write, &sa, 0);
    
    PROCESS_INFORMATION piM, piA, piP, piS;
    
    std::thread threadS(ProcessS, hPipeP_S_Read, hPipeS_Out_Write);
    
    std::thread threadP(ProcessP, hPipeA_P_Read, hPipeP_S_Write);
    
    std::thread threadA(ProcessA, hPipeM_A_Read, hPipeA_P_Write);
    
    std::thread threadM(ProcessM, GetStdHandle(STD_INPUT_HANDLE), hPipeM_A_Write);
    
    Sleep(100);
    
    std::cout << "Enter numbers separated by spaces (end with Enter): ";
    std::string input;
    std::getline(std::cin, input);
    
    HANDLE hInputPipe = GetStdHandle(STD_INPUT_HANDLE);
    WriteFile(hPipeM_A_Write, input.c_str(), input.size(), NULL, NULL);
    std::string eol = "\n";
    WriteFile(hPipeM_A_Write, eol.c_str(), eol.size(), NULL, NULL);
    
    threadM.join();
    threadA.join();
    threadP.join();
    threadS.join();

    char outputBuffer[BUFFER_SIZE];
    DWORD bytesRead;
    
    std::cout << "\nProcessing chain: M(x*7) -> A(x+14) -> P(x^3) -> S(sum)\n";
    std::cout << "Input numbers: " << input << std::endl;
    std::cout << "\nResult: ";
    
    if (ReadFile(hPipeS_Out_Read, outputBuffer, BUFFER_SIZE - 1, &bytesRead, NULL) && bytesRead > 0) {
        outputBuffer[bytesRead] = '\0';
        std::cout << outputBuffer;
    }
    
    CloseHandle(hPipeM_A_Read);
    CloseHandle(hPipeM_A_Write);
    CloseHandle(hPipeA_P_Read);
    CloseHandle(hPipeA_P_Write);
    CloseHandle(hPipeP_S_Read);
    CloseHandle(hPipeP_S_Write);
    CloseHandle(hPipeS_Out_Read);
    CloseHandle(hPipeS_Out_Write);
    
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    
    return 0;
}