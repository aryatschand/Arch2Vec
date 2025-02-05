#include "matmul_halide.h" // Include the generated header file
#include "HalideBuffer.h"  // Include the Halide Buffer class
#include <iostream>
#include <chrono> // For measuring execution time

int main() {
    const int M = 1024; // Rows in A and C
    const int N = 1024; // Columns in B and C
    const int K = 1024; // Columns in A and rows in B

    // Allocate input and output buffers
    Halide::Runtime::Buffer<float> A(M, K), B(K, N), C(M, N);

    // Initialize input matrices
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            A(i, j) = static_cast<float>(i + j);
        }
    }
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            B(i, j) = static_cast<float>(i - j);
        }
    }

    // Measure time for matrix multiplication
    auto start_time = std::chrono::high_resolution_clock::now();
    int error = matmul(A, B, C);
    auto end_time = std::chrono::high_resolution_clock::now();

    if (error) {
        std::cerr << "Halide returned an error: " << error << "\n";
        return -1;
    }

    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Matrix multiplication completed in " << elapsed.count() << " seconds!\n";

    return 0;
}
