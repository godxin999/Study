import os
from collections import defaultdict


def count_lines(file_path):
    with open(file_path, 'r', encoding='ISO-8859-1') as f:
        return len(f.readlines())


def walk_dir(dir_path):
    dir_lines = defaultdict(int)
    file_lines = []
    total_lines = 0

    for root, dirs, files in os.walk(dir_path):
        for file in files:
            if file.endswith('.cppm') or file.endswith('.cpp'):
                file_path = os.path.join(root, file)
                lines = count_lines(file_path)
                dir_lines[root] += lines
                file_lines.append((file_path, lines))
                total_lines += lines

        # 在当前目录下按行数对文件和子目录进行排序
        sorted_items = sorted(
            list(dir_lines.items()) + file_lines,
            key=lambda x: x[1],
            reverse=True
        )

        # 打印当前目录下的排序结果
        indent = ' ' * (root.count(os.sep) - dir_path.count(os.sep)) + '|--'
        print(
            f"{indent} {os.path.relpath(root, dir_path)} ({dir_lines[root]})")
        for path, lines in sorted_items:
            if path != root:
                indent = ' ' * (path.count(os.sep) -
                                dir_path.count(os.sep)) + '|--'
                print(f"{indent} {os.path.relpath(path, root)} ({lines})")

        # 清空当前目录下的行数统计，以便进行下一轮统计
        dir_lines.clear()
        file_lines.clear()

    print(f"\nTotal lines: {total_lines}")


walk_dir('.')
