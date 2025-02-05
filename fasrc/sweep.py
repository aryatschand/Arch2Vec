#!/usr/bin/env python3

import os
import subprocess
import multiprocessing

PIN_PATH = "./pin-3.26-98690-g1fc9d60e6-gcc-linux/pin"
DCACHE_SO = "pin-3.26-98690-g1fc9d60e6-gcc-linux/source/tools/Memory/obj-intel64/dcache.so"
PROGRAM_DIR = "./frequent_element"

# 1) L1 sizes (passed to -c, in KB)
L1_SIZES_KB = [1, 2, 4]

# 2) Block sizes (passed to -b, in bytes)
BLOCK_SIZES_B = [1, 4, 8]

# Our code binaries: code_1 through code_10
CODES = [f"code_{i}" for i in range(1, 6)]


def parse_total_miss_percentage(output_file):
    """
    Read the dcache output file, find the line starting with '# Total-Misses:',
    and return the floating-point percentage. Returns None if not found.
    """
    with open(output_file, "r") as f:
        for line in f:
            # Example line: "# Total-Misses:              3018    4.52%"
            if line.startswith("# Total-Misses:"):
                parts = line.split()
                # parts might be: ["#", "Total-Misses:", "3018", "4.52%"]
                pct_str = parts[3]  # "4.52%"
                return float(pct_str.replace("%", ""))
    return None


def run_simulation(task):
    """
    A worker function to run one simulation. 'task' is a tuple:
      (l1_size_kb, block_size_b, code_name)
    It:
      1. Builds a command to run Pin with the dcache tool.
      2. Executes it three times.
      3. Averages the Total-Misses % from the three runs.
    Returns (l1_size_kb, block_size_b, code_name, avg_miss_pct).
    """
    l1_size_kb, block_size_b, code = task

    # Construct an output filename that encodes our parameters
    out_file = f"output/dcache_{l1_size_kb}KB_{block_size_b}B_{code}.out"

    # Path to the program we run (e.g., ./sum_of_squares/code_1)
    prog_path = os.path.join(PROGRAM_DIR, code)

    # Add the command-line arguments that need to be passed to the program
    prog_args = ["1000", "1000", "1000"]

    # To store the miss percentages from the three runs
    miss_pct_list = []

    for _ in range(3):  # Repeat the simulation 3 times
        # Build the command
        cmd = [
            PIN_PATH,
            "-t", DCACHE_SO,
            "-o", out_file,
            "-c", str(l1_size_kb),
            "-b", str(block_size_b),
            "--", prog_path] + prog_args  # Append program arguments

        # Run the simulation
        subprocess.run(cmd, check=True)

        # Parse the Total-Misses %
        miss_pct = parse_total_miss_percentage(out_file)
        if miss_pct is not None:
            miss_pct_list.append(miss_pct)

    # Calculate the average miss percentage from the three runs
    if miss_pct_list:
        avg_miss_pct = sum(miss_pct_list) / len(miss_pct_list)
    else:
        avg_miss_pct = None  # If no valid miss percentage was found

    return (l1_size_kb, block_size_b, code, avg_miss_pct)




def main():
    # Create all tasks (9 configs × 10 programs = 90 tasks)
    tasks = []
    for l1_size_kb in L1_SIZES_KB:
        for block_size_b in BLOCK_SIZES_B:
            for code in CODES:
                tasks.append((l1_size_kb, block_size_b, code))

    # Use a multiprocessing Pool to distribute tasks
    with multiprocessing.Pool() as pool:
        results = pool.map(run_simulation, tasks)

    # results is a list of (l1_size_kb, block_size_b, code, miss_pct)

    # Organize results by (l1_size_kb, block_size_b)
    # We want for each config an array of (code, miss_pct).
    # Then we'll sort by miss_pct descending.
    config_dict = {}
    for (l1_size, block_size, code, miss_pct) in results:
        config_dict.setdefault((l1_size, block_size), []).append((code, miss_pct))

    # For each of the 9 config combos, sort the array from highest to lowest
    # by total miss percentage, then print
    for (l1_size, block_size) in sorted(config_dict.keys()):
        # Sort descending by miss_pct
        data = config_dict[(l1_size, block_size)]
        data.sort(key=lambda x: x[1] if x[1] is not None else 0, reverse=True)

        print(f"\n=== L1: {l1_size} KB, Block: {block_size} B ===")
        print("Sorted (highest -> lowest) by L1 total misses %:")
        
        # Print both the program name and its corresponding miss rate
        for program, miss_rate in data:
            print(f"{program}: {miss_rate:.2f}%")

if __name__ == "__main__":
    main()
