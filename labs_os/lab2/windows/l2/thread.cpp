#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

class block_multiplier_thread_input {
private:
    int n;
    std::vector<std::vector<int>> a, b;

public:
    block_multiplier_thread_input(int size) : n(size) {
        a.resize(n, std::vector<int>(n));
        b.resize(n, std::vector<int>(n));
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10);

        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) {
                a[i][j] = dis(gen);
                b[i][j] = dis(gen);
            }
    }

    void multiply_two_blocks(std::vector<std::vector<int>>& c,
                            const std::vector<std::vector<int>>& block_a,
                            const std::vector<std::vector<int>>& block_b,
                            int start_row, int start_col) {
        int block_a_cols = block_a[0].size();
        int block_a_rows = block_a.size();
        int block_b_cols = block_b[0].size();
        
        for (int i = 0; i < block_a_rows; i++) 
            for (int j = 0; j < block_b_cols; j++) {
                int sum = 0;
                for (int k = 0; k < block_a_cols; k++) {
                    int dd = block_a[i][k] * block_b[k][j];
                    sum += dd;
                }
                c[start_row + i][start_col + j] += sum;
            }
    }

    std::vector<std::vector<int>> get_block(const std::vector<std::vector<int>>& matrix,
                                           int block_i, int block_j, int block_size) {
        int start_i = block_i * block_size;
        int end_i = std::min((block_i + 1) * block_size, n);
        int start_j = block_j * block_size;
        int end_j = std::min((block_j + 1) * block_size, n);
        
        std::vector<std::vector<int>> block(end_i - start_i, std::vector<int>(end_j - start_j));
        
        for (int i = 0; i < end_i - start_i; i++) 
            for (int j = 0; j < end_j - start_j; j++) 
                block[i][j] = matrix[start_i + i][start_j + j];
        
        return block;
    }

    std::vector<std::vector<int>> multiply_with_blocks(int block_size) {
        std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
        std::vector<std::thread> threads;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        int blocks_per_side = (n + block_size - 1) / block_size;

        for (int i = 0; i < blocks_per_side; i++) 
            for (int j = 0; j < blocks_per_side; j++) 
                for (int k = 0; k < blocks_per_side; k++) {
                    auto block_a = get_block(a, i, k, block_size);
                    auto block_b = get_block(b, k, j, block_size);
                    
                    int start_row = i * block_size;
                    int start_col = j * block_size;
                    
                    threads.emplace_back([&, block_a, block_b, start_row, start_col]() {
                        multiply_two_blocks(c, block_a, block_b, start_row, start_col);
                    });
                }
        
        for (auto& thread : threads) 
            thread.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "block_size=" << block_size 
                  << ", total_blocks_A_B=" << blocks_per_side * blocks_per_side
                  << ", block_pairs=" << blocks_per_side * blocks_per_side * blocks_per_side
                  << ", threads=" << threads.size()
                  << ", time=" << duration.count() << " microseconds" << std::endl;
        
        return c;
    }

    std::vector<std::vector<int>> multiply_sequential() {
        std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) {
                int sum = 0;
                for (int k = 0; k < n; k++) {
                    int dd = a[i][k] * b[k][j];
                    sum += dd;
                }
                c[i][j] = sum;
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "sequential time=" << duration.count() << " microseconds" << std::endl;
        return c;
    }
};

int main() {
    int n;
    
    std::cout << "n = ";
    std::cin >> n;
    
    if (n < 10) {
        std::cout << "Matrix size must be at least 10 * 10" << std::endl;
        return 1;
    }
    
    block_multiplier_thread_input multiplier(n);
    std::cout << "\nMatrix size: " << n << "x" << n << std::endl;
    
    std::cout << "\nSequential multiplication:" << std::endl;
    auto sequential = multiplier.multiply_sequential();
    
    std::cout << "\nBlock multiplication:" << std::endl;

    for (int block_size = 1; block_size <= n; block_size++) {
        std::cout << "Testing block size " << block_size << ": ";
        auto block_result = multiplier.multiply_with_blocks(block_size);
    } 
    return 0;
}