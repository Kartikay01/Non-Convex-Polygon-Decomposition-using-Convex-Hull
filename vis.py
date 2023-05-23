import subprocess

# compile the C++ file
subprocess.run(['g++', 'DAAdox.cpp', '-o', 'a'])

# run the compiled executable and capture the output
output = subprocess.run('./a', capture_output=True, text=True)

# split the output into a list of lines
output_lines = output.stdout.split('\n')

# print the output lines
print(output_lines)
