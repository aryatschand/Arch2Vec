#include "Halide.h"
#include <iostream>
#include <cstdlib> // For std::atoi
#include <chrono> // For timing

using namespace Halide;

int main(int argc, char **argv) {
    // Retrieve optimization flags from command line arguments
    bool ENABLE_TILING = false;
    bool ENABLE_VECTORIZATION = false;
    bool ENABLE_PARALLELIZATION = false;
    bool ENABLE_UNROLLING = false;
    bool ENABLE_FUSION = false;
    bool ENABLE_SPLIT_PARALLEL = false;

    if (argc > 1) ENABLE_TILING = (std::atoi(argv[1]) != 0);
    if (argc > 2) ENABLE_VECTORIZATION = (std::atoi(argv[2]) != 0);
    if (argc > 3) ENABLE_PARALLELIZATION = (std::atoi(argv[3]) != 0);
    if (argc > 4) ENABLE_UNROLLING = (std::atoi(argv[4]) != 0);
    if (argc > 5) ENABLE_FUSION = (std::atoi(argv[5]) != 0);
    if (argc > 6) ENABLE_SPLIT_PARALLEL = (std::atoi(argv[6]) != 0);

    // Declare Halide variables
    Var x("x"), y("y"), k("k"), xi("xi"), yi("yi"), xo("xo"), yo("yo"), split_idx("split_idx");

    const int M = 1024; // Rows in A and C
    const int N = 1024; // Columns in B and C
    const int K = 1024; // Columns in A and rows in B

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

    // Halide function for matrix multiplication
    Func matmul("matmul");
    RDom r(0, K, "r");

    // Define the algorithm
    matmul(x, y) += A(x, r) * B(r, y);

    // Apply optimizations based on flags
    if (ENABLE_TILING) {
        matmul.tile(x, y, xo, yo, xi, yi, 32, 32) // Tiling with 32x32 blocks
              .reorder(xi, yi, xo, yo);

        matmul.update().tile(x, y, xo, yo, xi, yi, 32, 32)
              .reorder(xi, yi, xo, yo);

        if (ENABLE_UNROLLING) {
            matmul.unroll(xi, 4).unroll(yi, 4);
            matmul.update().unroll(xi, 4).unroll(yi, 4);
        }
        if (ENABLE_VECTORIZATION) {
            matmul.vectorize(xi, 8);
            matmul.update().vectorize(xi, 8);
        }
        if (ENABLE_PARALLELIZATION) {
            matmul.parallel(yo);
            matmul.update().parallel(yo);
        }
    } else {
        if (ENABLE_UNROLLING) {
            matmul.unroll(x, 4).unroll(y, 4);
            matmul.update().unroll(x, 4).unroll(y, 4);
        }
        if (ENABLE_VECTORIZATION) {
            matmul.vectorize(x, 4);
            matmul.update().vectorize(x, 4);
        }
        if (ENABLE_PARALLELIZATION) {
            matmul.parallel(y);
            matmul.update().parallel(y);
        }
    }

    if (ENABLE_FUSION) {
        // Corrected fusion optimization
        matmul.fuse(x, y, split_idx);
        matmul.parallel(split_idx).vectorize(split_idx, 8);
        matmul.update().fuse(x, y, split_idx).parallel(split_idx).vectorize(split_idx, 8);
    }

    if (ENABLE_SPLIT_PARALLEL) {
        if (!ENABLE_TILING) {
            // Apply split and parallelism normally when tiling is disabled
            matmul.split(y, yo, yi, 64)
                .parallel(yo)
                .vectorize(x, 8);

            matmul.update().split(y, yo, yi, 64)
                .parallel(yo)
                .vectorize(x, 8);
        } else {
            // Adapt split parallelism to work within tiled dimensions
            matmul.tile(x, y, xo, yo, xi, yi, 32, 32)  // Ensure tiling is consistent
                .split(yo, yo, yi, 16)               // Split the outer tile (yo)
                .parallel(yo)                        // Parallelize over outer tiles
                .vectorize(xi, 8);                   // Vectorize inner computation

            matmul.update().tile(x, y, xo, yo, xi, yi, 32, 32)
                        .split(yo, yo, yi, 16)       // Split the update loop's outer tile
                        .parallel(yo)                // Parallelize over the split tiles
                        .vectorize(xi, 8);           // Vectorize inner update computation
        }
    }


    // Measure time for .realize()
    auto start_time = std::chrono::high_resolution_clock::now();
    matmul.realize(C);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Matrix multiplication completed successfully in " << elapsed.count() << " seconds!\n";

    return 0;
}
