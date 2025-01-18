import subprocess
import random
import numpy as np
import matplotlib.pyplot as plt
import time

# Define 10 g++ optimization flags with values to sweep
flags = {
    "-funroll-loops": ["", "-funroll-loops"],
    "-fvectorize": ["", "-fno-vectorize"],
    "-ftree-vectorize": ["", "-fno-tree-vectorize"],
    "-fstrict-aliasing": ["", "-fno-strict-aliasing"],
    "-fprefetch-loop-arrays": ["", "-fno-prefetch-loop-arrays"],
    "-falign-functions": ["", "-falign-functions=32", "-falign-functions=64"],
    "-fmerge-all-constants": ["", "-fno-merge-all-constants"],
    "-flto": ["", "-flto=thin"],
    "-fno-exceptions": ["", "-fexceptions"],
    "-fno-rtti": ["", "-frtti"],
}

# Set random seed for reproducibility
random.seed(42)

# Number of combinations to test
num_combinations = 20

# Record speedups
speedups = []
specific_speedup = None

for i in range(num_combinations):
    # Randomly select one value for each flag
    selected_flags = [random.choice(values) for values in flags.values()]
    selected_flags_str = " ".join(filter(None, selected_flags))

    try:
        # Compile FOR LOOP binary with the selected flags
        subprocess.run(
            f"g++ -std=c++17 -march=native -mavx512f -O3 -ffast-math {selected_flags_str} -I/Users/aryatschand/Desktop/arch2vec/include/eigen for_loop.cpp -o for_loop",
            shell=True,
            check=True,
        )

        # Compile UNROLLED binary with the selected flags
        subprocess.run(
            f"g++ -std=c++17 -march=native -mavx512f -O3 -ffast-math {selected_flags_str}-I/Users/aryatschand/Desktop/arch2vec/include/eigen unrolled_eigen.cpp -o unrolled_eigen",
            shell=True,
            check=True,
        )

        # Measure runtime of FOR LOOP binary (10 runs)
        for_loop_runtimes = []
        for _ in range(10):
            start_time = time.time()
            subprocess.run("./for_loop", shell=True, check=True)
            for_loop_runtimes.append(time.time() - start_time)
        avg_runtime_for_loop = sum(for_loop_runtimes) / len(for_loop_runtimes)

        # Measure runtime of UNROLLED binary (10 runs)
        unrolled_runtimes = []
        for _ in range(10):
            start_time = time.time()
            subprocess.run("./unrolled_eigen", shell=True, check=True)
            unrolled_runtimes.append(time.time() - start_time)
        avg_runtime_unrolled = sum(unrolled_runtimes) / len(unrolled_runtimes)

        # Calculate speedup
        speedup = avg_runtime_for_loop / avg_runtime_unrolled
        speedups.append(speedup)

        print(f"Flags = {selected_flags_str}: Speedup = {speedup:.3f}")

    except subprocess.CalledProcessError as e:
        print(f"Error during compilation or execution for combination {i+1}. Skipping...")

# Test specific flags
specific_flags = "-fno-vectorize -finline -flto=thin -frtti"
try:
    # Compile FOR LOOP binary with specific flags
    subprocess.run(
        f"g++ -std=c++17 -march=native -mavx512f -O3 -ffast-math {specific_flags} -I/usr/local/include batched_dense_dense_for_loop.cpp -o for_loop",
        shell=True,
        check=True,
    )

    # Compile UNROLLED binary with specific flags
    subprocess.run(
        f"g++ -std=c++17 -march=native -mavx512f -O3 -ffast-math {specific_flags} -I/usr/local/include batched_dense_dense_unrolled_eigen.cpp -o unrolled_eigen",
        shell=True,
        check=True,
    )

    # Measure runtime of FOR LOOP binary (10 runs)
    for_loop_runtimes = []
    for _ in range(10):
        start_time = time.time()
        subprocess.run("./for_loop", shell=True, check=True)
        for_loop_runtimes.append(time.time() - start_time)
    avg_runtime_for_loop = sum(for_loop_runtimes) / len(for_loop_runtimes)

    # Measure runtime of UNROLLED binary (10 runs)
    unrolled_runtimes = []
    for _ in range(10):
        start_time = time.time()
        subprocess.run("./unrolled_eigen", shell=True, check=True)
        unrolled_runtimes.append(time.time() - start_time)
    avg_runtime_unrolled = sum(unrolled_runtimes) / len(unrolled_runtimes)

    # Calculate speedup
    specific_speedup = avg_runtime_for_loop / avg_runtime_unrolled
    speedups.append(specific_speedup)

    print(f"Specific Flags = {specific_flags}: Speedup = {specific_speedup:.3f}")

except subprocess.CalledProcessError as e:
    print(f"Error during compilation or execution with specific flags. Skipping...")

# Plot histogram of speedups
plt.hist(speedups, bins=20, edgecolor='black', color='blue')
plt.axvline(1.0, color='red', linestyle='--', label='1x Speedup')

# Highlight the specific speedup
if specific_speedup is not None:
    plt.hist([specific_speedup], bins=20, color='red', alpha=0.7, label='Specific Flags Speedup')

plt.xlabel("Speedup (UNROLLED / FOR LOOP)")
plt.ylabel("Frequency")
plt.title("Histogram of Speedups for Different g++ Optimization Flags")
plt.legend()
plt.show()
