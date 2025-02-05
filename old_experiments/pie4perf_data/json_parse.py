import json
import pandas as pd

# Path to your JSONL file
jsonl_file = 'val.jsonl'

# List to store each JSON entry
data = []

# Read the JSONL file
with open(jsonl_file, 'r') as file:
    for line in file:
        entry = json.loads(line.strip())  # Parse each line as JSON
        data.append(entry)

# Convert the list of dictionaries to a DataFrame
df = pd.DataFrame(data)

# Save the DataFrame to a CSV file
output_csv = 'val.csv'
df.to_csv(output_csv, index=False)

print(f"JSONL data has been converted and saved to {output_csv}")
