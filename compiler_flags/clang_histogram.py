import subprocess
import itertools
import time
import numpy as np
import matplotlib.pyplot as plt

# Define the 4 flags and their on/off values
flags = {
    "-fvectorize": ["-fvectorize", "-fno-vectorize"],
    "-funroll-loops": ["-fno-unroll-loops", "-funroll-loops"],
    "-fstrict-aliasing": ["-fstrict-aliasing", "-fno-strict-aliasing"],
    "-fmerge-all-constants": ["-fmerge-all-constants", "-fno-merge-all-constants"],
    "-ffast-math": ["-ffast-math", "-fno-fast-math"],
}


# Generate all combinations of the flags
flag_combinations = list(itertools.product(*flags.values()))
num_combinations = len(flag_combinations)

# Record runtimes and speedups
unrolled_runtimes = []

for i, combination in enumerate(flag_combinations):
    selected_flags_str = " ".join(combination)

    try:
        # Compile FOR LOOP binary with the selected flags
        subprocess.run(
            f"clang++ -std=c++17 -march=native -O3 {selected_flags_str} -I/Users/aryatschand/Documents/Github/Arch2Vec/compiler_flags/include/eigen for_loop.cpp -o for_loop",
            shell=True,
            check=True,
        )

        # Compile UNROLLED binary with the selected flags
        # subprocess.run(
        #     f"clang++ -std=c++17 -march=native -O3 {selected_flags_str} -I/Users/aryatschand/Documents/Github/Arch2Vec/compiler_flags/include/blaze unrolled_blaze.cpp -o unrolled_blaze",
        #     shell=True,
        #     check=True,
        # )

        # Measure runtime of UNROLLED binary (10 runs)
        blaze_runtimes = []
        for _ in range(10):
            start_time = time.time()
            subprocess.run("./for_loop", shell=True, check=True)
            blaze_runtimes.append(time.time() - start_time)

        avg_runtime_unrolled = sum(blaze_runtimes) / len(blaze_runtimes)
        unrolled_runtimes.append(avg_runtime_unrolled)

        print(f"Combination {i + 1}/{num_combinations}: {selected_flags_str} -> Avg Runtime = {avg_runtime_unrolled:.6f} seconds")

    except subprocess.CalledProcessError:
        print(f"Error during compilation or execution for combination {i + 1}. Skipping...")

# Determine the index of the combination where all flags are ON
all_flags_on = tuple([flag[0] for flag in flags.values()])
#all_flags_on_index = flag_combinations.index(all_flags_on)

# Plot histograms
plt.figure(figsize=(10, 6))
plt.hist(unrolled_runtimes, bins=16, edgecolor='black', color='green')
#plt.axvline(unrolled_runtimes[all_flags_on_index], color='red', linestyle='--', label='All Flags ON')
plt.xlabel("Runtime (s)")
plt.ylabel("Frequency")
plt.title("Histogram of FOR LOOP Runtimes")
plt.xlim(min(unrolled_runtimes), max(unrolled_runtimes))  # Dynamically scale X-axis
plt.legend()
plt.show()
