import os

total_lines = 0

current_path = os.getcwd()

for root, dirs, files in os.walk(current_path):
    for file in files:
        if file.endswith('.h') or file.endswith('.cpp'):
            with open(os.path.join(root, file), 'r', encoding='ISO-8859-1') as f:
                lines = len(f.readlines())
                total_lines += lines
                print(f"File: {os.path.join(root, file)}, Lines: {lines}")

print(f"Total lines: {total_lines}")