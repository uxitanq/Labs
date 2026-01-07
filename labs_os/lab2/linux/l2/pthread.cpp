#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <random>

class block_multiplier_pthread_input {
private:
    int n;
    std::vector<std::vector<int>> a, b;

    struct block_multiply_data {
        block_multiplier_pthread_input* self;
        std::vector<std::vector<int>>* result;
        std::vector<std::vector<int>> block_a;
        std::vector<std::vector<int>> block_b;
        int start_row, start_col;
    };

public:
    block_multiplier_pthread_input(int size) : n(size) {
        a.resize(n, std::vector<int>(n));
        b.resize(n, std::vector<int>(n));
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = dis(gen);
                b[i][j] = dis(gen);
            }
        }
    }

    static void* multiply_two_blocks_wrapper(void* arg) {
        block_multiply_data* data = static_cast<block_multiply_data*>(arg);
        
        int block_a_rows = data->block_a.size();
        int block_a_cols = data->block_a[0].size();
        int block_b_cols = data->block_b[0].size();
        
        for (int i = 0; i < block_a_rows; i++) {
            for (int j = 0; j < block_b_cols; j++) {
                int sum = 0;
                for (int k = 0; k < block_a_cols; k++) {
                    sum += data->block_a[i][k] * data->block_b[k][j];
                }
                (*data->result)[data->start_row + i][data->start_col + j] += sum;
            }
        }
        
        delete data;
        return nullptr;
    }

    std::vector<std::vector<int>> get_block(const std::vector<std::vector<int>>& matrix,
                                           int block_i, int block_j, int block_size) {
                int start_i = block_i * block_size;
        int end_i;
        if ((block_i + 1) * block_size <= n)
            end_i = (block_i + 1) * block_size;
        else 
            end_i = n;

        int start_j = block_j * block_size;
        int end_j;
        if ((block_j + 1) * block_size <= n)
            end_j = (block_j + 1) * block_size;
        else 
            end_j = n;
        
        std::vector<std::vector<int>> block(end_i - start_i, std::vector<int>(end_j - start_j));
        
        for (int i = 0; i < end_i - start_i; i++) 
            for (int j = 0; j < end_j - start_j; j++) 
                block[i][j] = matrix[start_i + i][start_j + j];
        
        return block;
    }

    std::vector<std::vector<int>> multiply_with_blocks(int block_size) {
        std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
        std::vector<pthread_t> threads;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        int blocks_per_side = (n + block_size - 1) / block_size;

        for (int i = 0; i < blocks_per_side; i++) 
            for (int j = 0; j < blocks_per_side; j++) 
                for (int k = 0; k < blocks_per_side; k++) {
                    auto block_a = get_block(a, i, k, block_size);
                    auto block_b = get_block(b, k, j, block_size);
                    
                    int start_row = i * block_size;
                    int start_col = j * block_size;
                    
                    block_multiply_data* data = new block_multiply_data{
                        this, &c, block_a, block_b, start_row, start_col
                    };
                    
                    pthread_t thread;
                    if (pthread_create(&thread, nullptr, multiply_two_blocks_wrapper, data) == 0) {
                        threads.push_back(thread);
                    } else 
                        delete data;
                }

        for (pthread_t thread : threads) 
            pthread_join(thread, nullptr);

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
        std::cout << "At least 10" << std::endl;
        return 1;
    }
    
    block_multiplier_pthread_input multiplier(n);
    std::cout << "\nMatrix size: " << n << "x" << n << std::endl;
    
    std::cout << "\nSequential multiplication:" << std::endl;
    auto sequential = multiplier.multiply_sequential();
    
    std::cout << "\nBlock multiplication:" << std::endl;
    
    std::cout << "Testing key block sizes:" << std::endl;
    
    if (n > 20){
        
        int test_sizes[] = {1, 2, 3, 5, n/10, n/5, n/4, n/3, n/2, n};
        for (int block_size : test_sizes) 
            if (block_size >= 1 && block_size <= n) {
                std::cout << "  block_size=" << block_size << ": ";
                auto block_result = multiplier.multiply_with_blocks(block_size);
            }
    }
    else {
        std::cout << "\nTesting all block sizes (1 to " << n << "):" << std::endl;
        for (int block_size = 1; block_size <= n; block_size++) {
            std::cout << "  block_size=" << block_size << ": ";
            auto block_result = multiplier.multiply_with_blocks(block_size);
        }
    }
    
    return 0;
}