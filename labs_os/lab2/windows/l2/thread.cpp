#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>

std::mutex resultMutex;

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

    void multiply_block_part(const std::vector<std::vector<int>>& matrixA,
                             const std::vector<std::vector<int>>& matrixB,
                             std::vector<std::vector<int>>& resultMatrix,
                             int blockRowA, int blockColA, int blockRowB, int blockColB, int blockSize) {
        
        int startRowA = blockRowA * blockSize;
        int endRowA;
        if (startRowA + blockSize > n) 
            endRowA = n;
        else 
            endRowA = startRowA + blockSize;
        
        int startColA = blockColA * blockSize;
        int endColA;
        if (startColA + blockSize > n) 
            endColA = n;
        else 
            endColA = startColA + blockSize;
        
        int startRowB = blockRowB * blockSize;
        int endRowB;
        if (startRowB + blockSize > n) 
            endRowB = n;
        else 
            endRowB = startRowB + blockSize;
        
        int startColB = blockColB * blockSize;
        int endColB;
        if (startColB + blockSize > n) 
            endColB = n;
        else 
            endColB = startColB + blockSize;

        int localRows = endRowA - startRowA;
        int localCols = endColB - startColB;
        std::vector<std::vector<int>> localResult(localRows, std::vector<int>(localCols, 0));

        int kEnd = endColA;
        if (endRowB < kEnd) 
            kEnd = endRowB;

        for (int i = startRowA; i < endRowA; ++i) {
            for (int j = startColB; j < endColB; ++j) {
                int sum = 0;
                for (int k = startColA; k < kEnd; ++k) {
                    int dd = matrixA[i][k] * matrixB[k][j];
                    sum += dd;
                }
                localResult[i - startRowA][j - startColB] = sum;
            }
        }

        std::lock_guard<std::mutex> lock(resultMutex);
        for (int i = 0; i < localRows; ++i) {
            for (int j = 0; j < localCols; ++j) {
                resultMatrix[startRowA + i][startColB + j] += localResult[i][j];
            }
        }
    }

    std::vector<std::vector<int>> multiply_block(int k) {
        std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        int blocks_per_dim = (n + k - 1) / k;
        size_t threads_created = 0;

        for (int block_i = 0; block_i < blocks_per_dim; ++block_i) {
            for (int block_j = 0; block_j < blocks_per_dim; ++block_j) {
                for (int block_k = 0; block_k < blocks_per_dim; ++block_k) {
                    threads.emplace_back([&, block_i, block_j, block_k]() {
                        multiply_block_part(a, b, c, block_i, block_k, block_k, block_j, k);
                    });
                    ++threads_created;
                }
            }
        }

        for (auto& thread : threads) 
            thread.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "threads: k=" << k << ", blocks=" << blocks_per_dim * blocks_per_dim * blocks_per_dim
                  << ", threads_created=" << threads_created
                  << ", time=" << duration.count() << " microseconds" << std::endl;
        
        return c;
    }

    std::vector<std::vector<int>> multiply_sequential() {
        std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) {
                c[i][j] = 0;
                for (int x = 0; x < n; x++) {
                    int dd = a[i][x] * b[x][j];
                    c[i][j] += dd; 
                }
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
    int n;
    
    std::cout << "n = ";
    std::cin >> n;
    
    if (n <= 0) {
        std::cout << "Error: matrix size must be positive!" << std::endl;
        return 1;
    }
    
    std::cout << "matrix size: " << n << "x" << n << std::endl;
    
    matrix_multiplier_thread multiplier(n);
    
    std::cout << "ordinary algorithm: ";
    auto sequential_result = multiplier.multiply_sequential();
    
    std::vector<int> k_values;
    for (int k : {1, 2, 5, 10, 20, 25, 50}) 
        if (k <= n) 
            k_values.push_back(k);
    
    if (k_values.size() < 3 && n > 1) {
        if (n/4 > 0) 
            k_values.push_back(n/4);
        if (n/2 > 0) 
            k_values.push_back(n/2);
        k_values.push_back(n);
    }
    
    for (int k : k_values) 
        if (k > 0 && k <= n) {
            auto thread_result = multiplier.multiply_block(k);
            if (multiplier.compare_matrices(sequential_result, thread_result)) 
                std::cout << "  result: correct" << std::endl;
            else 
                std::cout << "  result: matrices don't match!" << std::endl;
        }

    return 0;
}