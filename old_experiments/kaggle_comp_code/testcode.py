import requests
import subprocess
import time
import os
import re

# Define the URL for the test program
solution_url = "https://www.codechef.com/viewplaintext/5435866"

# Headers to mimic a browser
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
}

try:
    # Fetch the raw C++ code
    response = requests.get(solution_url, headers=headers)
    response.raise_for_status()
    
    # Extract the content inside the <pre> tags
    match = re.search(r"<pre>(.*?)</pre>", response.text, re.DOTALL)
    if not match:
        raise Exception("Failed to locate <pre> tags in the response.")

    # Decode HTML entities to get the clean C++ code
    from html import unescape
    code = unescape(match.group(1))

    # Create a directory for storing the source file and executable
    os.makedirs('test_code', exist_ok=True)

    # Save the code to a file
    code_file = "test_code/test_program.cpp"
    with open(code_file, 'w') as f:
        f.write(code)

    # Compile the code using g++
    executable = "test_code/test_program.out"
    compile_command = ["g++", "-O0", code_file, "-o", executable]
    subprocess.run(compile_command, check=True)

    # Run the executable and time its execution
    start_time = time.time()
    subprocess.run([executable], check=True)
    end_time = time.time()

    # Calculate the runtime
    runtime = end_time - start_time
    print(f"Program executed successfully in {runtime:.4f} seconds.")

except requests.exceptions.RequestException as e:
    print(f"Failed to fetch the solution from CodeChef: {e}")
except subprocess.CalledProcessError as e:
    print(f"Compilation or execution error: {e}")
except Exception as e:
    print(f"An unexpected error occurred: {e}")
