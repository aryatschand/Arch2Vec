#include "Halide.h"
#include <iostream>
#include <cstdlib> // For std::atoi

using namespace Halide;

int main(int argc, char **argv) {
    // Retrieve optimization flags from command line arguments
    bool ENABLE_TILING = false;
    bool ENABLE_VECTORIZATION = false;
    bool ENABLE_PARALLELIZATION = false;
    bool ENABLE_UNROLLING = false;

    if (argc > 1) ENABLE_TILING = (std::atoi(argv[1]) != 0);
    if (argc > 2) ENABLE_VECTORIZATION = (std::atoi(argv[2]) != 0);
    if (argc > 3) ENABLE_PARALLELIZATION = (std::atoi(argv[3]) != 0);
    if (argc > 4) ENABLE_UNROLLING = (std::atoi(argv[4]) != 0);

    // Declare Halide variables
    Var x("x"), y("y"), k("k"), xi("xi"), yi("yi"), xo("xo"), yo("yo");

    // Halide function for matrix multiplication
    Func matmul("matmul");
    RDom r(0, 1024, "r"); // Assuming a fixed size for simplicity

    // Input matrices
    ImageParam A(type_of<float>(), 2, "A");
    ImageParam B(type_of<float>(), 2, "B");

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

    // Compile to a static library
    matmul.compile_to_static_library("matmul_halide", {A, B}, "matmul");

    return 0;
}
