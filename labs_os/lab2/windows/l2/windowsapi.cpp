#include <iostream>
#include <vector>
#include <windows.h>
#include <chrono>
#include <random>
#include <mutex>
#include <queue>
#include <semaphore>

class block_multiplier_windows_input {
private:
    int n;
    std::vector<std::vector<int>> a, b;
    std::mutex result_mutex;
    std::mutex queue_mutex;
    std::queue<HANDLE> completed_threads;
    HANDLE semaphore;

    struct block_multiply_data {
        block_multiplier_windows_input* self;
        std::vector<std::vector<int>>* result;
        std::vector<std::vector<int>> block_a;
        std::vector<std::vector<int>> block_b;
        int start_row_a, start_col_a;
        int start_row_b, start_col_b; 
        int start_row_c, start_col_c; 
    };

public:
    block_multiplier_windows_input(int size) : n(size) {
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
        
        semaphore = CreateSemaphore(NULL, 16, 16, NULL); 
    }

    ~block_multiplier_windows_input() {
        CloseHandle(semaphore);
    }

    static DWORD WINAPI multiply_two_blocks_wrapper(LPVOID param) {
        block_multiply_data* data = (block_multiply_data*)param;
        
        int block_a_rows = data->block_a.size();
        int block_a_cols = (block_a_rows > 0) ? data->block_a[0].size() : 0;
        int block_b_cols = (data->block_b.size() > 0) ? data->block_b[0].size() : 0;
        
        std::vector<std::vector<int>> local_result(block_a_rows, 
                                                   std::vector<int>(block_b_cols, 0));
        
        for (int i = 0; i < block_a_rows; i++) {
            for (int j = 0; j < block_b_cols; j++) {
                int sum = 0;
                for (int k = 0; k < block_a_cols; k++) {
                    int dd = data->block_a[i][k] * data->block_b[k][j];
                    sum += dd;
                }
                local_result[i][j] = sum;
            }
        }
        
        {
            std::lock_guard<std::mutex> lock(data->self->result_mutex);
            for (int i = 0; i < block_a_rows; i++) {
                for (int j = 0; j < block_b_cols; j++) {
                    int global_i = data->start_row_c + i;
                    int global_j = data->start_col_c + j;
                    
                    if (global_i < data->result->size()) {
                        if (global_j < (*data->result)[global_i].size()) {
                            (*data->result)[global_i][global_j] += local_result[i][j];
                        }
                    }
                }
            }
        }
        
        ReleaseSemaphore(data->self->semaphore, 1, NULL);
        
        {
            std::lock_guard<std::mutex> lock(data->self->queue_mutex);
            data->self->completed_threads.push(GetCurrentThread());
        }
        
        delete data;
        return 0;
    }

    std::vector<std::vector<int>> get_block(const std::vector<std::vector<int>>& matrix,
                                           int block_i, int block_j, int block_size,
                                           int& start_i, int& start_j) {

        start_i = block_i * block_size;
        int end_i;
        if (start_i + block_size <= n)
            end_i = start_i + block_size;
        else 
            end_i = n;

        start_j = block_j * block_size;
        int end_j;
        if (start_j + block_size <= n)
            end_j = start_j + block_size;
        else 
            end_j = n;                  
        
        std::vector<std::vector<int>> block(end_i - start_i, 
                                           std::vector<int>(end_j - start_j));
        
        for (int i = 0; i < end_i - start_i; i++) 
            for (int j = 0; j < end_j - start_j; j++) 
                block[i][j] = matrix[start_i + i][start_j + j];
        
        return block;
    }

    std::vector<std::vector<int>> multiply_with_blocks(int block_size) {
        std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
        std::vector<HANDLE> threads;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        int blocks_per_side = (n + block_size - 1) / block_size;

        for (int i = 0; i < blocks_per_side; i++) {
            for (int j = 0; j < blocks_per_side; j++) {
                for (int k = 0; k < blocks_per_side; k++) {
                    WaitForSingleObject(semaphore, INFINITE);
                    
                    int start_row_a, start_col_a, start_row_b, start_col_b;
                    
                    auto block_a = get_block(a, i, k, block_size, start_row_a, start_col_a);
                    auto block_b = get_block(b, k, j, block_size, start_row_b, start_col_b);
                    
                    if (block_a.empty() || block_a[0].empty() || 
                        block_b.empty() || block_b[0].empty()) {
                        ReleaseSemaphore(semaphore, 1, NULL);
                        continue;
                    }
                    
                    int start_row_c = i * block_size;
                    int start_col_c = j * block_size;
                    
                    block_multiply_data* data = new block_multiply_data{
                        this, &c, block_a, block_b,
                        start_row_a, start_col_a,
                        start_row_b, start_col_b,
                        start_row_c, start_col_c 
                    };
                    
                    HANDLE thread = CreateThread(NULL, 0, 
                                                 multiply_two_blocks_wrapper, 
                                                 data, 0, NULL);
                    if (thread) 
                        threads.push_back(thread);
                    else {
                        delete data;
                        ReleaseSemaphore(semaphore, 1, NULL);
                    }
                }
            }
        }

        WaitForMultipleObjects(threads.size(), threads.data(), TRUE, INFINITE);

        for (HANDLE thread : threads) {
            CloseHandle(thread);
        }

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
    
    if (n < 5) {
        std::cout << "Matrix size must be at least 5 * 5" << std::endl;
        return 1;
    }
    
    block_multiplier_windows_input multiplier(n);
    std::cout << "\nMatrix size: " << n << "x" << n << std::endl;
    
    std::cout << "\nSequential multiplication:" << std::endl;
    auto sequential = multiplier.multiply_sequential();
    
    std::cout << "\nBlock multiplication:" << std::endl;
    
    int step;

    if (n/20 < 1)
        step = 1;
    else step = n/20;
        
    for (int block_size = 1; block_size <= n; block_size += step) {
        std::cout << "Testing block size " << block_size << ": ";
        auto block_result = multiplier.multiply_with_blocks(block_size);
    }    
    return 0;
}