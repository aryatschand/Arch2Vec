import os
import subprocess
import time

def generate_c_code(unroll=False, tile=False, transpose=False):
    """
    Generate C code for 3x3 matrix multiplication with specified optimizations.
    """
    c_code = """
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrix_multiply(int A[3][3], int B[3][3], int C[3][3]) {
"""

    if transpose:
        c_code += """
    // Transpose B for better memory access
    int B_T[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            B_T[j][i] = B[i][j];
        }
    }
"""

    if tile:
        c_code += """
    // Tiled Matrix Multiplication
    int block_size = 2;  // Example block size
    for (int ii = 0; ii < 3; ii += block_size) {
        for (int jj = 0; jj < 3; jj += block_size) {
            for (int kk = 0; kk < 3; kk += block_size) {
                for (int i = ii; i < ii + block_size && i < 3; i++) {
                    for (int j = jj; j < jj + block_size && j < 3; j++) {
                        for (int k = kk; k < kk + block_size && k < 3; k++) {
"""
        if transpose:
            c_code += "                            C[i][j] += A[i][k] * B_T[j][k];\n"
        else:
            c_code += "                            C[i][j] += A[i][k] * B[k][j];\n"
        c_code += """
                        }
                    }
                }
            }
        }
    }
"""

    if unroll:
        c_code += """
    // Fully Unrolled Matrix Multiplication
"""
        for i in range(3):
            for j in range(3):
                c_code += f"    C[{i}][{j}] = 0;\n"
                for k in range(3):
                    if transpose:
                        c_code += f"    C[{i}][{j}] += A[{i}][{k}] * B_T[{j}][{k}];\n"
                    else:
                        c_code += f"    C[{i}][{j}] += A[{i}][{k}] * B[{k}][{j}];\n"

    if not unroll and not tile:
        c_code += """
    // Basic Matrix Multiplication
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            C[i][j] = 0;
            for (int k = 0; k < 3; k++) {
"""
        if transpose:
            c_code += "                C[i][j] += A[i][k] * B_T[j][k];\n"
        else:
            c_code += "                C[i][j] += A[i][k] * B[k][j];\n"
        c_code += """
            }
        }
    }
"""

    # Close the function
    c_code += """
}

int main() {
    int A[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int B[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int C[3][3] = {0};

    clock_t start = clock();
    for (int iter = 0; iter < 1000000; iter++) {
        matrix_multiply(A, B, C);
    }
    clock_t end = clock();

    printf("Time: %f", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}
"""
    return c_code

def compile_and_run_c_code(c_code, filename):
    """
    Compile and run the given C code, returning the average execution time over 10 runs.
    """
    with open(filename, "w") as f:
        f.write(c_code)

    # Compile the C code
    binary_name = filename.replace(".c", "")
    subprocess.run(["clang", filename, "-o", binary_name, "-O0"])  # No compiler optimizations

    # Run the compiled code 10 times and collect execution times
    times = []
    for _ in range(10):
        result = subprocess.run([f"./{binary_name}"], capture_output=True, text=True)
        time_taken = float(result.stdout.strip().split()[-1])
        times.append(time_taken)

    # Cleanup
    os.remove(filename)
    os.remove(binary_name)

    return sum(times) / len(times)

def main():
    """
    Sweep all 9 combinations of optimizations and measure performance.
    """
    configs = [(unroll, tile, transpose) for unroll in [False, True]
                                      for tile in [False, True]
                                      for transpose in [False, True]]

    results = []
    for i, (unroll, tile, transpose) in enumerate(configs):
        print(f"Running configuration {i + 1}/9: unroll={unroll}, tile={tile}, transpose={transpose}")
        c_code = generate_c_code(unroll=unroll, tile=tile, transpose=transpose)
        avg_time = compile_and_run_c_code(c_code, f"matrix_mult_{i}.c")
        results.append((unroll, tile, transpose, avg_time))

    # Print results
    print("\nResults:")
    for unroll, tile, transpose, avg_time in results:
        print(f"unroll={unroll}, tile={tile}, transpose={transpose}, avg_time={avg_time:.6f} seconds")

if __name__ == "__main__":
    main()
