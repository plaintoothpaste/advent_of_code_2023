import argparse
from itertools import product


def count(array: list[str], row: int, col: int) -> int:
    """for a given location return the number of XMAS"""
    c = 0
    max_row = len(array) - 3
    max_col = len(array[0]) - 3
    if row > 2:
        # up
        s = array[row][col] + array[row - 1][col] + array[row - 2][col] + array[row - 3][col]
        c = c + 1 if s == "XMAS" else c

        if col > 2:
            # left + up
            s = array[row][col] + array[row - 1][col - 1] + array[row - 2][col - 2] + array[row - 3][col - 3]
            c = c + 1 if s == "XMAS" else c

        if col < max_col:
            # right + up
            s = array[row][col] + array[row - 1][col + 1] + array[row - 2][col + 2] + array[row - 3][col + 3]
            c = c + 1 if s == "XMAS" else c

    if row < max_row:
        # down
        s = array[row][col] + array[row + 1][col] + array[row + 2][col] + array[row + 3][col]
        c = c + 1 if s == "XMAS" else c

        if col > 2:
            # left + down
            s = array[row][col] + array[row + 1][col - 1] + array[row + 2][col - 2] + array[row + 3][col - 3]
            c = c + 1 if s == "XMAS" else c

        if col < max_col:
            # right + down
            s = array[row][col] + array[row + 1][col + 1] + array[row + 2][col + 2] + array[row + 3][col + 3]
            c = c + 1 if s == "XMAS" else c

    if col > 2:
        # left
        s = array[row][col] + array[row][col - 1] + array[row][col - 2] + array[row][col - 3]
        c = c + 1 if s == "XMAS" else c

    if col < max_col:
        # right
        s = array[row][col] + array[row][col + 1] + array[row][col + 2] + array[row][col + 3]
        c = c + 1 if s == "XMAS" else c

    return c


def load(file: str) -> list[str]:
    with open(file, "r") as fid:
        data = fid.readlines()
    return [d.strip() for d in data]


def main(array) -> int:
    final = 0
    for r, c in product(range(len(array)), range(len(array[0]))):
        if array[r][c] == "X":
            final += count(array, r, c)
    return final


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="A tool to solve aoc 2024 challenge")

    parser.add_argument("-i", "--input", help="input file", default="real.txt")
    args = parser.parse_args()
    data = load(args.input)
    res = main(data)
    print(res)
