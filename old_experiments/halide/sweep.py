import subprocess
import itertools
import matplotlib.pyplot as plt

# Define the combinations of flags
flags = ["ENABLE_TILING", "ENABLE_VECTORIZATION", "ENABLE_PARALLELIZATION", "ENABLE_UNROLLING"]
all_combinations = list(itertools.product([0, 1], repeat=len(flags)))

# Store runtimes for each combination
runtimes = []

# Path to the AOT-compiled binary generator and the runtime binary
generate_binary = "./matmul_generate"
run_binary = "./matmul_run"

# Sweep through all flag combinations
for combination in all_combinations:
    # Prepare the flag arguments for the generator binary
    flag_args = [str(flag) for flag in combination]
    
    # Run the generator binary to create the AOT-compiled library
    generate_command = [generate_binary] + flag_args
    subprocess.run(generate_command, check=True)
    
    # Compile the runtime binary with the generated static library
    compile_command = [
        "g++", "matmul_run.cpp", "matmul_halide.a",
        "-std=c++17", "-O3",
        "-I", "/opt/anaconda3/lib/python3.12/site-packages/halide/include",
        "-lpthread", "-ldl", "-o", "matmul_run"
    ]
    subprocess.run(compile_command, check=True)

    # Measure runtime of the compiled program
    runtime_list = []
    for _ in range(11):  # Run 11 times and take the average of 10 runs
        result = subprocess.run(
            [run_binary],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=True
        )
        
        # Parse the output for the runtime
        output_lines = result.stdout.splitlines()
        for line in output_lines:
            if "Matrix multiplication completed in" in line:
                runtime = float(line.split(" ")[-2])  # Extract runtime in seconds
                runtime_list.append(runtime)
                break
    
    # Remove the first warm-up run and compute the average
    runtime_list.pop(0)
    avg_runtime = sum(runtime_list) / len(runtime_list)
    runtimes.append(avg_runtime)
    print(f"Combination {combination} -> Runtime: {avg_runtime:.3f} seconds")

# Find the runtime for all flags enabled
all_flags_true_runtime = runtimes[-1]  # The last combination is (1, 1, 1, 1)

# Plot the histogram of runtimes
plt.figure(figsize=(10, 6))
plt.hist(runtimes, bins=16, alpha=0.75, edgecolor='black')
plt.axvline(all_flags_true_runtime, color='red', linestyle='--', label='All Flags True Runtime')
plt.title('Runtimes for Different Optimization Flag Combinations')
plt.xlabel('Runtime (seconds)')
plt.ylabel('Frequency')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)

# Show the plot
plt.show()
