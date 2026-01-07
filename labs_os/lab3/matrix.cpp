#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iomanip>
#include <functional>
#include "Buffered_channel.h"

using namespace std;

template<typename T>
struct MatrixBlock {
    vector<vector<T>> data;
    int startRow;
    int startCol;
    int blockRow;  
    int blockCol; 
};

struct BlockMultiplicationTask {
    MatrixBlock<int> blockA;
    MatrixBlock<int> blockB;
    int resultBlockRow;  
    int resultBlockCol;
};

struct BlockResult {
    vector<vector<int>> data;
    int resultBlockRow;
    int resultBlockCol;
};

vector<vector<int>> globalA;
vector<vector<int>> globalB;
int globalN = 0;

class MatrixMultiplier {
private:
    vector<vector<int>> A;
    vector<vector<int>> B;
    vector<vector<int>> C;
    int N;
    int k;
    
    vector<MatrixBlock<int>> splitMatrixIntoBlocks(const vector<vector<int>>& matrix, 
                                                   bool isMatrixA) const {
        vector<MatrixBlock<int>> blocks;
        
        int numRowBlocks = (N + k - 1) / k;
        int numColBlocks = (N + k - 1) / k;

        if (isMatrixA) {
            for (int blockRow = 0; blockRow < numRowBlocks; blockRow++) {
                int startRow = blockRow * k;
                int blockHeight = min(k, N - startRow);
                
                MatrixBlock<int> block;
                block.startRow = startRow;
                block.startCol = 0;  
                block.blockRow = blockRow;
                block.blockCol = 0;  
                
                block.data.resize(blockHeight, vector<int>(N));
                for (int i = 0; i < blockHeight; i++) 
                    for (int j = 0; j < N; j++) 
                        block.data[i][j] = matrix[startRow + i][j];

                blocks.push_back(block);
            }
        } else {
            for (int blockCol = 0; blockCol < numColBlocks; blockCol++) {
                int startCol = blockCol * k;
                int blockWidth = min(k, N - startCol);
                
                MatrixBlock<int> block;
                block.startRow = 0;  
                block.startCol = startCol;
                block.blockRow = 0;  
                block.blockCol = blockCol;
                
                block.data.resize(N, vector<int>(blockWidth));
                for (int i = 0; i < N; i++) 
                    for (int j = 0; j < blockWidth; j++) 
                        block.data[i][j] = matrix[i][startCol + j];

                blocks.push_back(block);
            }
        }
        
        return blocks;
    }
    
    BlockResult multiplyBlocks(const BlockMultiplicationTask& task) {
        const auto& blockA = task.blockA;
        const auto& blockB = task.blockB;
        
        int blockAHeight = blockA.data.size();    
        int blockAWidth = blockA.data[0].size();  
        int blockBHeight = blockB.data.size();
        int blockBWidth = blockB.data[0].size();  
        
        vector<vector<int>> blockResult(blockAHeight, vector<int>(blockBWidth, 0));
        
        for (int i = 0; i < blockAHeight; i++) 
            for (int j = 0; j < blockBWidth; j++) {
                int sum = 0;
                for (int l = 0; l < N; l++) {
                    int dd = blockA.data[i][l] * blockB.data[l][j];
                    sum += dd;
                }
                blockResult[i][j] = sum;
            }
        
        BlockResult result;
        result.data = std::move(blockResult);
        result.resultBlockRow = task.resultBlockRow;
        result.resultBlockCol = task.resultBlockCol;
        
        return result;
    }
    
public:
    MatrixMultiplier(int n, int blockSize) : N(n), k(blockSize) {
        A.resize(N, vector<int>(N, 0));
        B.resize(N, vector<int>(N, 0));
        C.resize(N, vector<int>(N, 0));
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 10);
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = dis(gen);
                B[i][j] = dis(gen);
            }
        }
        
        globalA = A;
        globalB = B;
        globalN = N;
    }
    
    vector<vector<int>> sequentialMultiply() {
        auto start = chrono::high_resolution_clock::now();
        
        vector<vector<int>> result(N, vector<int>(N, 0));
        
        for (int i = 0; i < N; i++) 
            for (int j = 0; j < N; j++) 
                for (int l = 0; l < N; l++) {
                    int dd = A[i][l] * B[l][j];
                    result[i][j] += dd;
                }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Sequential multiplication time: " << duration.count() << " microseconds" << endl;
        
        return result;
    }
    
    static BlockResult multiplyBlocksStatic(const BlockMultiplicationTask& task) {
        const auto& blockA = task.blockA;
        const auto& blockB = task.blockB;
        
        int blockAHeight = blockA.data.size();  
        int blockAWidth = blockA.data[0].size();  
        int blockBHeight = blockB.data.size();     
        int blockBWidth = blockB.data[0].size();   
        
        vector<vector<int>> blockResult(blockAHeight, vector<int>(blockBWidth, 0));
        
        for (int i = 0; i < blockAHeight; i++) 
            for (int j = 0; j < blockBWidth; j++) {
                int sum = 0;
                for (int l = 0; l < globalN; l++) {
                    int dd = blockA.data[i][l] * blockB.data[l][j];
                    sum += dd;
                }
                blockResult[i][j] = sum;
            }
        
        BlockResult result;
        result.data = std::move(blockResult);
        result.resultBlockRow = task.resultBlockRow;
        result.resultBlockCol = task.resultBlockCol;
        
        return result;
    }
    
    static void workerThread(BufferedChannel<BlockMultiplicationTask>* taskChannel,
                            BufferedChannel<BlockResult>* resultChannel) {
        while (true) {
            auto taskPair = taskChannel->Recv();
            if (!taskPair.second) 
                break; 
            
            BlockMultiplicationTask task = taskPair.first;
            BlockResult result = multiplyBlocksStatic(task);
            resultChannel->Send(std::move(result));
        }
    }
    
    vector<vector<int>> parallelMultiplyWithChannels() {
        auto start = chrono::high_resolution_clock::now();
        
        auto blocksA = splitMatrixIntoBlocks(A, true); 
        auto blocksB = splitMatrixIntoBlocks(B, false); 
        
        int numBlocksA = blocksA.size(); 
        int numBlocksB = blocksB.size(); 
        int totalTasks = numBlocksA * numBlocksB;  
        
        BufferedChannel<BlockMultiplicationTask> taskChannel(totalTasks);
        BufferedChannel<BlockResult> resultChannel(totalTasks);
        
        vector<thread> workers;
        for (int i = 0; i < totalTasks; i++) 
            workers.emplace_back(workerThread, &taskChannel, &resultChannel);
        
        for (int i = 0; i < numBlocksA; i++) 
            for (int j = 0; j < numBlocksB; j++) {
                BlockMultiplicationTask task;
                task.blockA = blocksA[i];
                task.blockB = blocksB[j];
                task.resultBlockRow = i; 
                task.resultBlockCol = j;
                
                taskChannel.Send(task);
            }
        
        taskChannel.Close();
        
        vector<vector<int>> result(N, vector<int>(N, 0));
        
        for (int i = 0; i < totalTasks; i++) {
            auto resultPair = resultChannel.Recv();
            if (resultPair.second) {
                BlockResult blockResult = resultPair.first;
                
                int startRow = blockResult.resultBlockRow * k;
                int startCol = blockResult.resultBlockCol * k;
                
                int blockHeight = blockResult.data.size();
                int blockWidth = blockResult.data[0].size();
                
                for (int r = 0; r < blockHeight; r++) 
                    for (int c = 0; c < blockWidth; c++) 
                        result[startRow + r][startCol + c] = blockResult.data[r][c];
                                        
            }
        }

        resultChannel.Close();
        
        for (auto& worker : workers) 
            worker.join();
        
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "Parallel multiplication with block size " << k 
             << " (blocks A: " << numBlocksA << ", blocks B: " << numBlocksB 
             << ", total tasks: " << totalTasks << ") time: " 
             << duration.count() << " microseconds" << endl;
        
        return result;
    }
    
    void comparePerformance() {
        cout << "\nMatrix size: " << N << "x" << N << endl;
        cout << "Block size: " << k << endl;
        
        auto seqResult = sequentialMultiply();
        
        auto parResult = parallelMultiplyWithChannels();
        
        bool correct = true;
        for (int i = 0; i < N && correct; i++) 
            for (int j = 0; j < N; j++) 
                if (seqResult[i][j] != parResult[i][j]) {
                    cout << "Mismatch at [" << i << "][" << j << "]: " 
                         << seqResult[i][j] << " != " << parResult[i][j] << endl;
                    correct = false;
                    break;
                }
        
        cout << "Results are " << (correct ? "correct" : "incorrect") << endl;
    }
    
    void printMatrix(const vector<vector<int>>& matrix, const string& name) {
        cout << "\nMatrix " << name << ":" << endl;
        for (int i = 0; i < min(5, N); i++) {
            for (int j = 0; j < min(5, N); j++) 
                cout << setw(4) << matrix[i][j] << " ";
            if (N > 5) cout << "...";
            cout << endl;
        }
        if (N > 5) cout << "... (showing first 5x5)" << endl;
    }
    
    void printBlockInfo() {
        auto blocksA = splitMatrixIntoBlocks(A, true);
        auto blocksB = splitMatrixIntoBlocks(B, false);
        
        cout << "\nBlock information:" << endl;
        cout << "Matrix A split into " << blocksA.size() << " row blocks" << endl;
        cout << "Matrix B split into " << blocksB.size() << " column blocks" << endl;
        cout << "Total multiplication tasks: " << blocksA.size() * blocksB.size() << endl;
    
        if (!blocksA.empty()) 
            cout << "Block A[0] size: " << blocksA[0].data.size() 
                 << "x" << blocksA[0].data[0].size() << endl;
        if (!blocksB.empty()) 
            cout << "Block B[0] size: " << blocksB[0].data.size() 
                 << "x" << blocksB[0].data[0].size() << endl;
    }
    
    const vector<vector<int>>& getMatrixA() const { return A; }
    const vector<vector<int>>& getMatrixB() const { return B; }
};

int main() {
    int n;
    std::cout << "n = ";
    std::cin >> n;
    if (n < 5) {
        std::cout << "n should be greater than 5" << std::endl;
        return 0;
    }
    for (int block_size = 1; block_size <= n; block_size++) {
        MatrixMultiplier multiplier(n, block_size);
        multiplier.printBlockInfo();

        if (block_size == 1) {
            multiplier.printMatrix(multiplier.getMatrixA(), "A");
            multiplier.printMatrix(multiplier.getMatrixB(), "B");
        }

        multiplier.comparePerformance();
    } 
    
    return 0;
}