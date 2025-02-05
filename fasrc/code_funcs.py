import os
import json
import glob
import re
import shutil
import logging
import subprocess
import time
from typing import Dict, List, Tuple, Optional


def parse_jsonl(jsonl_path: str) -> List[dict]:
    """ Parse a JSONL file (one JSON object per line) and return a list of records. """
    records = []
    with open(jsonl_path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            record = json.loads(line)
            records.append(record)
    return records


def create_bin_dir_for_record(record: dict, base_bin_dir: str, code_variant: str) -> str:
    """ Create a bin folder for each code variant (src/tgt) and return its path. """
    bin_dirname = f"{record['problem_id']}_{record[f'{code_variant}_id']}"
    bin_path = os.path.join(base_bin_dir, bin_dirname)
    os.makedirs(bin_path, exist_ok=True)
    return bin_path


def write_cpp_code(record: dict, bin_path: str, code_variant: str) -> str:
    """ Write the C++ source code to a .cpp file, replacing 'bits/stdc++.h' if necessary. """
    code_str = record[f"{code_variant}_code"]
    code_filename = f"{record[f'{code_variant}_id']}.cpp"
    code_path = os.path.join(bin_path, code_filename)

    # Replace bits/stdc++.h with standard headers
    if "#include <bits/stdc++.h>" in code_str:
        logging.warning(f"Replacing <bits/stdc++.h> in {code_filename}")
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

    if "#include<bits/stdc++.h>" in code_str:
        logging.warning(f"Replacing <bits/stdc++.h> in {code_filename}")
        code_str = code_str.replace(
            "#include<bits/stdc++.h>", 
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

    with open(code_path, 'w', encoding='utf-8') as f:
        f.write(code_str)

    return code_path


def copy_test_files(problem_id: str, bin_path: str, testcases_dir: str, testcase_indices: List[int] = [0]):
    """ Copy input/output test files to the bin directory. """
    problem_dir = os.path.join(testcases_dir, problem_id)
    
    for idx in testcase_indices:
        in_filename = f"input.{idx}.txt"
        out_filename = f"output.{idx}.txt"

        src_in_path = os.path.join(problem_dir, in_filename)
        src_out_path = os.path.join(problem_dir, out_filename)

        if not os.path.exists(src_in_path) or not os.path.exists(src_out_path):
            logging.warning(f"Missing input/output for problem {problem_id}, testcase {idx}")
            continue

        shutil.copyfile(src_in_path, os.path.join(bin_path, in_filename))
        shutil.copyfile(src_out_path, os.path.join(bin_path, out_filename))


def compile_cpp_code(code_path: str, timeout: int = 30, cflags: str = "--std=c++17 -O3") -> str:
    """ Compile the C++ source code into an executable binary. """
    output_path = os.path.splitext(code_path)[0] + ".out"
    
    cmd = ["/usr/bin/g++", code_path, "-o", output_path] + cflags.split()
    logging.info(f"Compiling: {' '.join(cmd)}")

    p = subprocess.run(cmd, capture_output=True, timeout=timeout, text=True)
    if p.returncode != 0:
        raise Exception(f"Compilation failed for {code_path}.\nStderr: {p.stderr}")
    
    return output_path


def get_accuracy(predicted_output: str, ground_truth_output: str) -> float:
    """ Compare program output with ground truth to determine correctness. """
    return 1.0 if predicted_output.strip() == ground_truth_output.strip() else 0.0


def exec_bin_for_acc(bin_path: str, in_path: str, ground_truth_output: str, timeout: int = 10) -> float:
    """ Execute compiled binary with input file and check correctness. """
    logging.info(f"Executing {bin_path} with input {in_path}")
    
    with open(in_path, 'r') as fh:
        p = subprocess.run([bin_path], capture_output=True, timeout=timeout, stdin=fh, text=True)
    
    if p.returncode != 0:
        raise Exception(f"Execution error for {bin_path}.\nStderr: {p.stderr}")
    
    return get_accuracy(p.stdout, ground_truth_output)


def compile_and_check_outputs(code_path: str, problem_id: str, testcases_dir: str, testcase_indices: List[int] = [0], timeout: int = 10) -> Tuple[Optional[str], Dict[int, float]]:
    """ Compile the C++ file and check its accuracy against testcases. """
    
    input_output_pairs = {}
    for idx in testcase_indices:
        in_path = os.path.join(testcases_dir, problem_id, f"input.{idx}.txt")
        out_path = os.path.join(testcases_dir, problem_id, f"output.{idx}.txt")
        if os.path.exists(in_path) and os.path.exists(out_path):
            input_output_pairs[idx] = (in_path, out_path)

    if not input_output_pairs:
        logging.warning(f"No valid testcases found for problem {problem_id}")
        return None, {}

    # Compile
    try:
        bin_path = compile_cpp_code(code_path, timeout=timeout)
        logging.info(f"Compiled {code_path} to {bin_path}")
    except Exception as e:
        logging.error(f"Compilation failed for {code_path}: {e}")
        return None, {idx: 0.0 for idx in input_output_pairs.keys()}

    # Run and evaluate
    accs = {}
    for idx, (in_path, out_path) in input_output_pairs.items():
        with open(out_path, 'r') as fh:
            ground_truth_output = fh.read()

        try:
            accs[idx] = exec_bin_for_acc(bin_path, in_path, ground_truth_output, timeout)
        except Exception as e:
            logging.error(f"Execution failed for {bin_path}: {e}")
            accs[idx] = 0.0

    return bin_path, accs


def main():
    """ Main function to process both `src_code` and `tgt_code` for all records, reading one at a time. """
    logging.basicConfig(level=logging.INFO)

    # Input paths
    jsonl_path = "../data/train.jsonl"
    testcases_dir = "../data/codenet/public_test_cases"
    base_bin_dir = "../bin"
    
    max_records = 10  # Cap processing to 10 records
    processed_count = 0

    # Read JSONL file one line at a time
    with open(jsonl_path, 'r', encoding='utf-8') as f:
        for line in f:
            if processed_count >= max_records:
                break  # Stop processing after reaching the cap

            line = line.strip()
            if not line:
                continue  # Skip empty lines
            
            try:
                record = json.loads(line)
            except json.JSONDecodeError as e:
                logging.error(f"Skipping invalid JSON entry: {e}")
                continue

            problem_id = record["problem_id"]

            for code_variant in ["src", "tgt"]:
                bin_path = create_bin_dir_for_record(record, base_bin_dir, code_variant)
                code_path = write_cpp_code(record, bin_path, code_variant)

                # Copy input/output files (assuming they are the same for both src and tgt)
                copy_test_files(problem_id, bin_path, testcases_dir, [0])

                # Compile and run
                bin_exe, accs = compile_and_check_outputs(code_path, problem_id, testcases_dir, [0], timeout=5)

                # Log results
                logging.info(f"Processed {code_variant}_code for {problem_id}, binary: {bin_exe}, accuracies: {accs}")

            processed_count += 1


if __name__ == "__main__":
    main()
