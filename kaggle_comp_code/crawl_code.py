import pandas as pd
import subprocess
import time
import matplotlib.pyplot as plt
import os

# Load the CSV files
data = pd.read_csv('solutions.csv')  # Replace 'solutions.csv' with your actual file name
solutions_data = pd.read_csv('solutions_code.csv')  # Replace with the CSV containing SolutionID and Solutions

# Filter rows where QCode is A3, Status is 'accepted', and Language contains 'C++'
filtered_data = data[(data['QCode'] == 'A3') & (data['Status'] == 'accepted') & (data['Language'].str.contains('C\+\+', na=False))]

runtimes = []
renamed_good_programs = []

# Create a directory for storing the source files and executables
os.makedirs('code_files', exist_ok=True)

for index, row in filtered_data.iterrows():
    solution_id = row['SolutionID']

    solution_code_row = solutions_data[solutions_data['SolutionID'] == solution_id]

    if not solution_code_row.empty:
        code = solution_code_row.iloc[0]['Solutions']

        # Save the code to a file
        code_file = f"code_files/solution_{index}.cpp"
        with open(code_file, 'w') as f:
            f.write(code)

        # Compile the code using g++ with -O0
        executable = f"code_files/solution_{index}.out"
        compile_command = ["g++", "-O0", code_file, "-o", executable]
        try:
            subprocess.run(compile_command, check=True)

            # Input data to test the program
            input_data = """
            3
            2
            < 100 No
            > 100 No
            3
            < 2 Yes
            > 4 Yes
            = 3 No
            6
            < 2 Yes
            > 1 Yes
            = 1 Yes
            = 1 Yes
            > 1 Yes
            = 1 Yes
            """

            # Run the program 3 times and record the average runtime
            total_runtime = 0
            start_time = time.time()
            for _ in range(5):
                process = subprocess.run([executable], input=input_data, text=True, capture_output=True, timeout=3)
            end_time = time.time()
            total_runtime += (end_time - start_time)

            # Only include programs that run successfully and within the timeout
            renamed_id = len(renamed_good_programs) + 1
            renamed_good_programs.append(0)
            runtimes.append((renamed_id, total_runtime))  # Store renamed ID (1-based) and runtime

            # Print the output
            print(f"Output for {solution_id}:", process.stdout)
        except subprocess.TimeoutExpired:
            print(f"Program {solution_id} took too long to run and was excluded.")
        except subprocess.CalledProcessError as e:
            print(f"Compilation or execution error for solution {solution_id}: {e}")
        except Exception as e:
            print(f"Error processing solution {solution_id}: {e}")
    else:
        print(f"No matching code found for SolutionID: {solution_id}")

# Sort runtimes and print sorted IDs
sorted_runtimes = sorted(runtimes, key=lambda x: x[1])
print("Sorted IDs by runtime:")
for renamed_id, runtime in sorted_runtimes:
    print(f"ID: {renamed_id}, Runtime: {runtime:.4f} seconds")

# Plot a histogram of the runtimes
plt.figure(figsize=(10, 6))
plt.hist([runtime for _, runtime in sorted_runtimes], bins=20, edgecolor='black')
plt.title('Histogram of Runtimes for A3 Accepted Solutions')
plt.xlabel('Average Runtime (seconds)')
plt.ylabel('Frequency')
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.show()
