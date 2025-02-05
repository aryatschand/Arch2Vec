import subprocess
import random
import numpy as np
import matplotlib.pyplot as plt
import time

#flags = "-fvectorize -ftree-vectorize -fstrict-aliasing -fmerge-all-constants"
flags = "-O3 -ffast-math -fno-unroll-loops"
#print(f"clang++ -std=c++17 -fno-unroll-loops -march=native -O3 {flags} -I/Users/aryatschand/Desktop/arch2vec/include/eigen for_loop.cpp -o for_loop")
#print(f"clang++ -std=c++17 -fno-unroll-loops -march=native {flags} -I/Users/aryatschand/Desktop/arch2vec/include/eigen unrolled_eigen.cpp -o unrolled_eigen")
subprocess.run(
    f"clang++ -std=c++17 -march=armv8.5-a -mcpu=apple-m3 {flags} -I/Users/aryatschand/Desktop/arch2vec/include/eigen for_loop.cpp -o for_loop",
    shell=True,
    check=True,
)

# Compile UNROLLED binary with the selected flags
subprocess.run(
    f"clang++ -std=c++17 -march=armv8.5-a -mcpu=apple-m3 {flags} -I/Users/aryatschand/Desktop/arch2vec/include/eigen unrolled_eigen.cpp -o unrolled_eigen",
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
#print(for_loop_runtimes, unrolled_runtimes)

#print("unroll flag")
print("for loop code" + str(avg_runtime_for_loop))
print("unrolled code" + str(avg_runtime_unrolled))

# Calculate speedup
print(avg_runtime_for_loop / avg_runtime_unrolled)
