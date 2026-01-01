#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    std::cout << "Enter numbers: ";
    std::string input;
    std::getline(std::cin, input);
    
    if (input.empty()) {
        input = "1 2 3";
        std::cout << "Using default: " << input << std::endl;
    }
    
    std::stringstream ss(input);
    std::vector<int> numbers;
    int num;
    while (ss >> num) 
        numbers.push_back(num);
    
    int pipe1[2], pipe2[2], pipe3[2];
    pipe(pipe1);
    pipe(pipe2);
    pipe(pipe3);
    
    pid_t pids[4];
    
    pids[0] = fork();
    if (pids[0] == 0) {
        close(pipe1[0]);
        std::stringstream output;
        for (int n : numbers) 
            output << (n * 7) << " ";
        std::string result = output.str();
        write(pipe1[1], result.c_str(), result.length() + 1);
        close(pipe1[1]);
        exit(0);
    }

    pids[1] = fork();
    if (pids[1] == 0) {
        close(pipe1[1]);
        close(pipe2[0]);
        
        char buffer[1024];
        read(pipe1[0], buffer, sizeof(buffer));
        
        std::stringstream input_ss(buffer);
        std::stringstream output;
        int num;
        
        while (input_ss >> num) 
            output << (num + 14) << " ";
        
        std::string result = output.str();
        write(pipe2[1], result.c_str(), result.length() + 1);
        
        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    }
    
    pids[2] = fork();
    if (pids[2] == 0) {
        close(pipe2[1]);
        close(pipe3[0]);
        
        char buffer[1024];
        read(pipe2[0], buffer, sizeof(buffer));
        
        std::stringstream input_ss(buffer);
        std::stringstream output;
        int num;
        
        while (input_ss >> num) 
            output << (num * num * num) << " ";
        
        std::string result = output.str();
        write(pipe3[1], result.c_str(), result.length() + 1);
        
        close(pipe2[0]);
        close(pipe3[1]);
        exit(0);
    }
    
    pids[3] = fork();
    if (pids[3] == 0) {
        close(pipe3[1]);
        
        char buffer[1024];
        read(pipe3[0], buffer, sizeof(buffer));
        
        std::stringstream input_ss(buffer);
        int sum = 0;
        int num;
        
        while (input_ss >> num) 
            sum += num;

        std::cout << "\nFinal sum: " << sum << std::endl;
        
        close(pipe3[0]);
        exit(0);
    }
    
    close(pipe1[0]); close(pipe1[1]);
    close(pipe2[0]); close(pipe2[1]);
    close(pipe3[0]); close(pipe3[1]);
    
    for (int i = 0; i < 4; i++) 
        waitpid(pids[i], NULL, 0);
    
    return 0;
}