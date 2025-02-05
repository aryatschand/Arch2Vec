#include "Halide.h"
#include <iostream>

using namespace Halide;

int main(int argc, char **argv) {
    const int M = 128; // Rows in A and C
    const int N = 128; // Columns in B and C
    const int K = 128; // Columns in A and rows in B

    // Declare the input matrices
    Buffer<float> A(M, K), B(K, N), C(M, N);

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

    // Halide variables
    Var x("x"), y("y"), k("k");

    // Halide function for matrix multiplication
    Func matmul("matmul");
    RDom r(0, K, "r");

    // Define the algorithm
    matmul(x, y) += A(x, r) * B(r, y);

    // Apply unrolling
    matmul.unroll(x, 4); // Unroll the x loop by a factor of 4

    // Schedule the update definition
    //matmul.update().unroll(x, 4); // Unroll the x loop by a factor of 4 in the update

    // Debugging: Print the loop nest to verify scheduling
    std::cout << "Loop nest before realization:\n";
    matmul.print_loop_nest();

    // Realize the function
    matmul.realize(C);

    std::cout << "Matrix multiplication completed successfully!\n";
    return 0;
}
