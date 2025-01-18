import subprocess
import os

# Path to the existing C++ source file
cpp_file_path = "generated_code/solution_S3573424.cpp"

# Check if the file exists
if not os.path.exists(cpp_file_path):
    print(f"File not found: {cpp_file_path}")
else:
    # Create a directory for the executable
    os.makedirs('test_code', exist_ok=True)

    # Define the executable path
    executable = "test_code/test_program.out"

    # Compile the code using g++
    compile_command = ["g++", "-O0", cpp_file_path, "-o", executable]
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

        # Run the executable with the input data
        process = subprocess.run([executable], input=input_data, text=True, capture_output=True)

        # Print the output
        print("Output:\n", process.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Compilation or execution error: {e}")
