#!/usr/bin/env python3

import os
import json
import csv
import logging
import subprocess
import time
import shutil

# ------------------ CONSTANTS / GLOBALS ------------------
PIN_PATH = "./pin-3.26-98690-g1fc9d60e6-gcc-linux/pin"
DCACHE_SO = "pin-3.26-98690-g1fc9d60e6-gcc-linux/source/tools/Memory/obj-intel64/dcache.so"

L1_SIZES_KB = [1, 2, 4]
BLOCK_SIZES_B = [1, 2, 4]
ASSOCIATIVITIES = [1, 2, 4]

# We'll read up to 100 lines from the JSONL => 200 codes total (src + tgt).
MAX_RECORDS = 100

BASE_BIN_DIR = "./bin"
TESTCASES_DIR = "./data/codenet/public_test_cases"
TRAIN_JSONL_PATH = "./data/train.jsonl"

CSV_OUTPUT = "pin_results.csv"

logging.basicConfig(level=logging.INFO)


# ------------------ HELPER FUNCTIONS ------------------

def parse_total_miss_percentage(output_file: str) -> float:
    """
    Reads the dcache output file, looks for the line starting with '# Total-Misses:',
    and returns the floating-point percentage (e.g. 4.52).
    Returns None if not found.
    """
    try:
        with open(output_file, "r", encoding="utf-8") as f:
            for line in f:
                if line.startswith("# Total-Misses:"):
                    # Example line:
                    #   "# Total-Misses:              3018    4.52%"
                    parts = line.split()
                    # parts -> ["#", "Total-Misses:", "3018", "4.52%"]
                    pct_str = parts[3]  # e.g. "4.52%"
                    return float(pct_str.replace("%", ""))
    except FileNotFoundError:
        pass
    return None


def get_accuracy(predicted_output: str, ground_truth_output: str) -> float:
    """
    Compare predicted_output vs ground_truth_output and return 1.0 if they match,
    else 0.0.
    """
    if predicted_output.strip() == ground_truth_output.strip():
        return 1.0
    return 0.0


def compile_cpp_code(code_str: str, code_id: str, bin_dir: str,
                     timeout: int = 30,
                     cflags: str = "--std=c++17 -O3") -> str:
    """
    Writes code_str to a .cpp file, compiles it, and returns the path to the .out binary.
    Also removes or replaces any #include <bits/stdc++.h> if needed.
    """
    # Replace bits/stdc++.h if needed
    if "#include <bits/stdc++.h>" in code_str:
        logging.warning(f"Replacing <bits/stdc++.h> in code_id={code_id}")
        code_str = code_str.replace(
            "#include <bits/stdc++.h>",
            """#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <bitset>
using namespace std;"""
        )

    # Write out the .cpp file
    cpp_path = os.path.join(bin_dir, f"{code_id}.cpp")
    with open(cpp_path, "w", encoding="utf-8") as f:
        f.write(code_str)

    # Compile
    out_path = os.path.splitext(cpp_path)[0] + ".out"
    cmd = ["/usr/bin/g++", cpp_path, "-o", out_path] + cflags.split()
    logging.info("Compiling: " + " ".join(cmd))

    p = subprocess.run(cmd, capture_output=True, timeout=timeout, text=True)
    if p.returncode != 0:
        raise RuntimeError(f"Compilation failed for {cpp_path}.\nStderr: {p.stderr}")

    # Wait a little if needed, ensuring the file is visible
    while not os.path.exists(out_path):
        time.sleep(0.05)

    return out_path


def copy_test_files(problem_id: str, bin_path: str, testcase_index: int = 0):
    """
    Copy input.X.txt / output.X.txt from TESTCASES_DIR/{problem_id}/
    to bin_path. By default, uses 'input.0.txt' / 'output.0.txt'.
    """
    problem_dir = os.path.join(TESTCASES_DIR, problem_id)

    in_filename = f"input.{testcase_index}.txt"
    out_filename = f"output.{testcase_index}.txt"

    src_in_path = os.path.join(problem_dir, in_filename)
    src_out_path = os.path.join(problem_dir, out_filename)

    if not os.path.exists(src_in_path) or not os.path.exists(src_out_path):
        logging.warning(f"Missing input/output for problem {problem_id}, testcase {testcase_index}")
        return

    dst_in_path = os.path.join(bin_path, in_filename)
    dst_out_path = os.path.join(bin_path, out_filename)
    shutil.copyfile(src_in_path, dst_in_path)
    shutil.copyfile(src_out_path, dst_out_path)


def run_pin_simulation(bin_exe: str,
                       code_id: str,
                       problem_id: str,
                       l1_size_kb: int,
                       block_size_b: int,
                       assoc: int,
                       input_file: str,
                       output_file: str = None,
                       repeat_runs: int = 1) -> (float, float): # type: ignore
    """
    Runs Pin + dcache on the compiled binary, passing input_file via stdin.
    - If output_file is provided, we compare the program's stdout to that for correctness.
    - Returns (avg_miss_pct, avg_accuracy).
    """
    bin_dir = os.path.dirname(bin_exe)
    # We'll store each run's dcache results in a file named by config + code_id
    dcache_out_file = os.path.join(
        bin_dir, f"dcache_{l1_size_kb}KB_{block_size_b}B_{assoc}A_{code_id}.out"
    )

    miss_pct_list = []
    accuracy_list = []

    for _ in range(repeat_runs):
        cmd = [
            PIN_PATH,
            "-t", DCACHE_SO,
            "-o", dcache_out_file,
            "-c", str(l1_size_kb),
            "-b", str(block_size_b),
            "-a", str(assoc),
            "--", bin_exe
        ]
        logging.info(f"Running Pin on {bin_exe} with input={input_file}")

        # Feed input_file into the program
        with open(input_file, 'r') as infile:
            p = subprocess.run(cmd, stdin=infile, capture_output=True, text=True)

        if p.returncode != 0:
            logging.error(f"Pin run failed for {bin_exe}, stderr:\n{p.stderr}")
            continue

        # 1) parse dcache stats
        miss_pct = parse_total_miss_percentage(dcache_out_file)
        if miss_pct is not None:
            miss_pct_list.append(miss_pct)

        # 2) (optional) compare output
        if output_file and os.path.exists(output_file):
            with open(output_file, 'r') as fh:
                gt_output = fh.read()
            acc = get_accuracy(p.stdout, gt_output)
            accuracy_list.append(acc)
        else:
            accuracy_list.append(0.0)

    if miss_pct_list:
        avg_miss_pct = sum(miss_pct_list) / len(miss_pct_list)
    else:
        avg_miss_pct = None

    if accuracy_list:
        avg_acc = sum(accuracy_list) / len(accuracy_list)
    else:
        avg_acc = None

    return (avg_miss_pct, avg_acc)


# ------------------ MAIN ------------------

def main():
    """
    1) Read up to 100 lines from train.jsonl, giving us 200 code objects (src + tgt).
    2) For each code:
       - create bin folder
       - compile
       - copy test files (input.0.txt / output.0.txt)
       - run 27 pin simulations with input.0.txt
       - parse and record miss rates + optional correctness
    3) Write all results to pin_results.csv
    """
    results = []
    record_count = 0

    with open(TRAIN_JSONL_PATH, "r", encoding="utf-8") as f:
        for line in f:
            if record_count >= MAX_RECORDS:
                break

            line = line.strip()
            if not line:
                continue

            try:
                record = json.loads(line)
            except json.JSONDecodeError as e:
                logging.error(f"Skipping invalid JSON line: {e}")
                continue

            problem_id = record["problem_id"]
            # We'll handle 2 code variants
            variants = [("src", record["src_id"], record["src_code"]),
                        ("tgt", record["tgt_id"], record["tgt_code"])]

            for (variant_name, code_id, code_str) in variants:
                bin_subdir = f"{problem_id}_{code_id}"
                bin_path = os.path.join(BASE_BIN_DIR, bin_subdir)
                os.makedirs(bin_path, exist_ok=True)

                # Compile
                try:
                    bin_exe = compile_cpp_code(code_str, code_id, bin_path)
                except RuntimeError as e:
                    logging.error(f"Compilation error for code_id={code_id}: {e}")
                    continue

                # Copy input.0.txt / output.0.txt
                copy_test_files(problem_id, bin_path, testcase_index=0)
                in_file = os.path.join(bin_path, "input.0.txt")
                out_file = os.path.join(bin_path, "output.0.txt")

                # If input file doesn't exist, skip
                if not os.path.exists(in_file):
                    logging.warning(f"No input file for code_id={code_id}, skipping")
                    continue

                # Sweep 27 configs: L1 size, block size, associativity
                for l1_kb in L1_SIZES_KB:
                    for block_b in BLOCK_SIZES_B:
                        for assoc in ASSOCIATIVITIES:
                            miss_pct, accuracy = run_pin_simulation(
                                bin_exe=bin_exe,
                                code_id=code_id,
                                problem_id=problem_id,
                                l1_size_kb=l1_kb,
                                block_size_b=block_b,
                                assoc=assoc,
                                input_file=in_file,
                                output_file=out_file,   # for correctness
                                repeat_runs=1          # or 3 if you want multiple runs
                            )

                            row = {
                                "code_id": code_id,
                                "problem_id": problem_id,
                                "variant": variant_name,
                                "l1_size_kb": l1_kb,
                                "block_size_b": block_b,
                                "associativity": assoc,
                                "miss_pct": miss_pct if miss_pct is not None else -1.0,
                                "accuracy": accuracy if accuracy is not None else -1.0
                            }
                            results.append(row)

            record_count += 1

    # Finally, write everything to CSV
    fieldnames = [
        "code_id", "problem_id", "variant",
        "l1_size_kb", "block_size_b", "associativity",
        "miss_pct", "accuracy"
    ]
    with open(CSV_OUTPUT, "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for row in results:
            writer.writerow(row)

    logging.info(f"Done. Wrote {len(results)} rows to {CSV_OUTPUT}")


if __name__ == "__main__":
    main()
