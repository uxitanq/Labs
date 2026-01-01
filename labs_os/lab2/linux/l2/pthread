#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <random>
#include <atomic>

class simple_block_matrix_multiplier {
private:
    int n;
    std::vector<std::vector<int>> a, b;
    std::atomic<int> active_threads;

    struct block_params {
        simple_block_matrix_multiplier* self;
        std::vector<std::vector<int>>* result;
        int start_row, end_row;
        int start_col, end_col;
    };

public:
    simple_block_matrix_multiplier(int size) : n(size), active_threads(0) {
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

    static void* block_function(void* arg) {
        block_params* p = static_cast<block_params*>(arg);
        int n = p->self->n;
        
        for (int i = p->start_row; i < p->end_row; i++) {
            for (int j = p->start_col; j < p->end_col; j++) {
                int sum = 0;
                for (int k = 0; k < n; k++) 
                    sum += p->self->a[i][k] * p->self->b[k][j];
                (*p->result)[i][j] = sum;
            }
        }
        
        p->self->active_threads--;
        delete p;
        return nullptr;
    }

    std::vector<std::vector<int>> multiply_with_blocks(int block_size) {
        std::vector<std::vector<int>> result(n, std::vector<int>(n, 0));
        std::vector<pthread_t> threads;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        int blocks_per_side = (n + block_size - 1) / block_size;
        int max_concurrent_threads = 8; 

        for (int i = 0; i < blocks_per_side; i++) {
            for (int j = 0; j < blocks_per_side; j++) {
                while (active_threads.load() >= max_concurrent_threads) {
                }

                int start_row = i * block_size;
                int end_row = std::min((i + 1) * block_size, n);
                int start_col = j * block_size;
                int end_col = std::min((j + 1) * block_size, n);

                block_params* params = new block_params;
                params->self = this;
                params->result = &result;
                params->start_row = start_row;
                params->end_row = end_row;
                params->start_col = start_col;
                params->end_col = end_col;
                
                active_threads++;
                
                pthread_t thread;
                if (pthread_create(&thread, nullptr, block_function, params) == 0)
                    threads.push_back(thread);
                else {
                    active_threads--;
                    delete params;
                    std::cout << "error: failed to create thread" << std::endl;
                }
            }
        }
        
        for (pthread_t thread : threads) 
            pthread_join(thread, nullptr);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "block multiplication: block_size=" << block_size 
                  << ", blocks=" << blocks_per_side * blocks_per_side
                  << ", threads=" << threads.size()
                  << ", time=" << duration.count() << " microseconds" << std::endl;
        
        return result;
    }

    std::vector<std::vector<int>> multiply_sequential() {
        std::vector<std::vector<int>> result(n, std::vector<int>(n, 0));
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int sum = 0;
                for (int k = 0; k < n; k++) 
                    sum += a[i][k] * b[k][j];
                result[i][j] = sum;
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "sequential: time=" << duration.count() << " microseconds" << std::endl;
        return result;
    }

    bool compare_results(const std::vector<std::vector<int>>& r1, const std::vector<std::vector<int>>& r2) {
        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) 
                if (r1[i][j] != r2[i][j]) 
                    return false;
        return true;
    }
};

int main() {
    const int n = 100;
    std::cout << "matrix size: " << n << "x" << n << std::endl;
    
    simple_block_matrix_multiplier multiplier(n);
    
    auto sequential = multiplier.multiply_sequential();
    std::cout << "sequential result[0][0] = " << sequential[0][0] << std::endl << std::endl;
    
    for (int block_size : {50, 25, 20, 10, 5, 2, 1}) 
        if (block_size <= n) {
            auto block_result = multiplier.multiply_with_blocks(block_size);
            std::cout << "result[0][0] = " << block_result[0][0];
            
            if (multiplier.compare_results(sequential, block_result)) 
                std::cout << " - correct" << std::endl;
            else std::cout << " - ERROR" << std::endl;
        }

    return 0;
}