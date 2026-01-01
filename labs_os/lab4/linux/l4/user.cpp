#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
#include <chrono>
#include <thread>

bool IsProcessRunning(const std::string& processName) {
    DIR* procDir = opendir("/proc");
    
    if (!procDir) 
        return false;
    
    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            char* endptr;
            pid_t pid = strtol(entry->d_name, &endptr, 10);
            
            if (*endptr == '\0') {
                std::string commPath = "/proc/" + std::to_string(pid) + "/comm";
                std::ifstream commFile(commPath);
                
                if (commFile.is_open()) {
                    std::string name;
                    std::getline(commFile, name);
                    
                    if (name == processName) {
                        closedir(procDir);
                        return true;
                    }
                }
            }
        }
    }
    
    closedir(procDir);
    return false;
}

pid_t GetProcessIdByName(const std::string& processName) {
    DIR* procDir = opendir("/proc");
    
    if (!procDir) return 0;
    
    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            char* endptr;
            pid_t pid = strtol(entry->d_name, &endptr, 10);
            
            if (*endptr == '\0') {
                std::string commPath = "/proc/" + std::to_string(pid) + "/comm";
                std::ifstream commFile(commPath);
                
                if (commFile.is_open()) {
                    std::string name;
                    std::getline(commFile, name);
                    
                    if (name == processName) {
                        closedir(procDir);
                        return pid;
                    }
                }
            }
        }
    }
    
    closedir(procDir);
    return 0;
}

int CountProcessesByName(const std::string& processName) {
    int count = 0;
    DIR* procDir = opendir("/proc");
    
    if (!procDir) 
        return 0;
    
    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            char* endptr;
            pid_t pid = strtol(entry->d_name, &endptr, 10);
            
            if (*endptr == '\0') {
                std::string commPath = "/proc/" + std::to_string(pid) + "/comm";
                std::ifstream commFile(commPath);
                
                if (commFile.is_open()) {
                    std::string name;
                    std::getline(commFile, name);
                    
                    if (name == processName) {
                        count++;
                        std::cout << "  Found: " << name << " (PID: " << pid << ")" << std::endl;
                    }
                }
            }
        }
    }
    
    closedir(procDir);
    return count;
}

int main() {
    std::string processesToKill = "gedit";
    std::cout << "Setting environment variable PROC_TO_KILL = " << processesToKill << std::endl;
    
    if (setenv("PROC_TO_KILL", processesToKill.c_str(), 1) != 0) {
        std::cout << "Failed to set environment variable!" << std::endl;
        return 1;
    }
    
    std::cout << "\nStarting test processes (gedit only)..." << std::endl;
    
    system("gedit --new-window &");
    std::cout << "Started: Gedit #1" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    system("gedit --new-window &");
    std::cout << "Started: Gedit #2" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    system("gedit --new-window &");
    std::cout << "Started: Gedit #3" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "\nChecking if gedit processes are running..." << std::endl;
    
    bool geditRunning = IsProcessRunning("gedit");
    std::cout << "Gedit processes are running: " << (geditRunning ? "YES" : "NO") << std::endl;
    
    int geditCount = CountProcessesByName("gedit");
    std::cout << "Total gedit processes: " << geditCount << std::endl;
    
    std::cout << "\n 1) Killing by name " << std::endl;
    std::cout << "Running: ./killer --name gedit" << std::endl;
    system("./killer --name gedit");
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    int geditCountAfter = CountProcessesByName("gedit");
    std::cout << "Gedit processes after killing by name: " << geditCountAfter << std::endl;
    
    std::cout << "\nStarting more gedit processes for next test..." << std::endl;
    system("gedit --new-window &");
    system("gedit --new-window &");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "\n 2) Killing from environment variable " << std::endl;
    
    int beforeCount = CountProcessesByName("gedit");
    std::cout << "Gedit processes before: " << beforeCount << std::endl;
    std::cout << "Running: ./killer (without parameters)" << std::endl;
    system("./killer");
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "\nFinal status:" << std::endl;
    bool finalStatus = IsProcessRunning("gedit");
    std::cout << "Gedit processes are running: " << (finalStatus ? "YES" : "NO") << std::endl;
    
    std::cout << "\nCleaning up environment variable..." << std::endl;
    if (unsetenv("PROC_TO_KILL") == 0) 
        std::cout << "Environment variable PROC_TO_KILL removed successfully" << std::endl;
    else 
        std::cout << "Failed to remove environment variable!" << std::endl;
    
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}