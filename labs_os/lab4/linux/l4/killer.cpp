#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>

bool KillProcessById(pid_t processId) {
    if (kill(processId, SIGKILL) == 0) {
        std::cout << "Successfully terminated process with ID: " << processId << std::endl;
        return true;
    } else {
        std::cout << "Failed to terminate process with ID: " << processId 
                  << " (Error: " << strerror(errno) << ")" << std::endl;
        return false;
    }
}

bool ProcessExists(pid_t pid) {
    return kill(pid, 0) == 0;
}

std::string GetProcessName(pid_t pid) {
    std::string commPath = "/proc/" + std::to_string(pid) + "/comm";
    std::ifstream commFile(commPath);
    
    if (commFile.is_open()) {
        std::string name;
        std::getline(commFile, name);
        return name;
    }
    
    return "";
}

bool KillProcessesByName(const std::string& processName) {
    bool found = false;
    DIR* procDir = opendir("/proc");
    
    if (!procDir) {
        std::cout << "Failed to open /proc directory" << std::endl;
        return false;
    }
    
    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            char* endptr;
            pid_t pid = strtol(entry->d_name, &endptr, 10);
            
            if (*endptr == '\0') {  
                std::string name = GetProcessName(pid);
                
                if (!name.empty() && name == processName) {
                    std::cout << "Found process: " << name << " (PID: " << pid << ")" << std::endl;
                    if (KillProcessById(pid)) 
                        found = true;
                }
            }
        }
    }
    
    closedir(procDir);
    
    if (found == false) 
        std::cout << "No processes found with name: " << processName << std::endl;
    
    return found;
}

std::vector<std::string> GetProcessesFromEnvironment() {
    std::vector<std::string> processes;
    const char* envValue = std::getenv("PROC_TO_KILL");
    
    if (envValue != nullptr) {
        std::string envStr = envValue;
        std::stringstream ss(envStr);
        std::string processName;
        
        while (std::getline(ss, processName, ',')) {
            size_t start = processName.find_first_not_of(" \t");
            size_t end = processName.find_last_not_of(" \t");
            if (start != std::string::npos && end != std::string::npos) {
                processName = processName.substr(start, end - start + 1);
                
                if (!processName.empty()) 
                    processes.push_back(processName);
            }
        }
    }
    
    return processes;
}

int main(int argc, char* argv[]) {
    std::cout << "killer" << std::endl;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--id" && i + 1 < argc) {
            pid_t processId = std::stoi(argv[++i]);
            std::cout << "Killing process by ID: " << processId << std::endl;
            KillProcessById(processId);
        }
        else if (arg == "--name" && i + 1 < argc) {
            std::string processName = argv[++i];
            std::cout << "Killing processes by name: " << processName << std::endl;
            KillProcessesByName(processName);
        }
        else if (arg == "--help") {
            std::cout << "Usage:" << std::endl;
            std::cout << "  ./killer --id <process_id>" << std::endl;
            std::cout << "  ./killer --name <process_name>" << std::endl;
            std::cout << "  ./killer (will kill processes from PROC_TO_KILL env var)" << std::endl;
        }
    }
    
    if (argc == 1) {
        std::cout << "Checking PROC_TO_KILL environment variable..." << std::endl;
        auto processes = GetProcessesFromEnvironment();
        
        if (!processes.empty()) {
            std::cout << "Found processes to kill from environment variable:" << std::endl;
            for (const auto& proc : processes) 
                std::cout << "  - " << proc << std::endl;
            
            std::cout << "\nKilling processes..." << std::endl;
            for (const auto& proc : processes) 
                KillProcessesByName(proc);
        } else std::cout << "No processes specified in PROC_TO_KILL environment variable" << std::endl;
    }
    
    return 0;
}