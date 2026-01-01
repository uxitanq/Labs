#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

class matrix_multiplier_thread {
private:
    std::vector<std::vector<int>> a, b;
    int n;

public:
    matrix_multiplier_thread(int size) : n(size) {
        a.resize(n, std::vector<int>(n));
        b.resize(n, std::vector<int>(n));
        initialize_matrices();
    }

    void initialize_matrices() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10);

        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) {
                a[i][j] = dis(gen);
                b[i][j] = dis(gen);
            }
    }

    std::vector<std::vector<int>> multiply_block(int k) {
        std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        int num_blocks = (n + k - 1) / k;

        for (int i = 0; i < num_blocks; i++) 
            for (int j = 0; j < num_blocks; j++) {
                int start_row = i * k;
                int end_row = std::min((i + 1) * k, n);
                int start_col = j * k;
                int end_col = std::min((j + 1) * k, n);

                threads.emplace_back([&, start_row, end_row, start_col, end_col]() {
                    for (int i2 = start_row; i2 < end_row; i2++) 
                        for (int j2 = start_col; j2 < end_col; j2++) {
                            c[i2][j2] = 0;
                            for (int x = 0; x < n; x++) 
                                c[i2][j2] += a[i2][x] * b[x][j2];
                        }
                });
            }

        for (auto& thread : threads) 
            thread.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "threads: k=" << k << ", blocks=" << num_blocks * num_blocks 
                  << ", time=" << duration.count() << " microseconds" << std::endl;
        
        return c;
    }

    std::vector<std::vector<int>> multiply_sequential() {
        std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) {
                c[i][j] = 0;
                for (int x = 0; x < n; x++) 
                    c[i][j] += a[i][x] * b[x][j];
            }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "sequential: time=" << duration.count() << " microseconds" << std::endl;
        return c;
    }

    bool compare_matrices(const std::vector<std::vector<int>>& c1, const std::vector<std::vector<int>>& c2) {
        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) 
                if (c1[i][j] != c2[i][j]) 
                    return false;
        return true;
    }
};

int main() {
    const int n = 100;
    std::cout << "matrix size: " << n << "x" << n << std::endl;
    
    matrix_multiplier_thread multiplier(n);
    
    std::cout << "ordinary algorithm: ";
    auto sequential_result = multiplier.multiply_sequential();
    
    for (int k : {1, 2, 5, 10, 20, 25, 50}) 
        if (k <= n) {
            auto thread_result = multiplier.multiply_block(k);
            if (multiplier.compare_matrices(sequential_result, thread_result)) 
                std::cout << "  result: correct" << std::endl;
            else 
                std::cout << "  result: matrices don't match!" << std::endl;
        }

    return 0;
}