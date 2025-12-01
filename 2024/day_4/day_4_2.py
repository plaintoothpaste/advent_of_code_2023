import argparse
from itertools import product


def window(array: list[str], row: int, col: int) -> int:
    """for a given window verify that it contains MAS
    middle letter must be A row and col is middle point
    """
    assert array[row][col]=="A"

    d1 = array[row - 1][col-1] + array[row+1][col+1]
    d2 = array[row + 1][col-1] + array[row-1][col+1]
    
    if (d1 == "MS" or d1 == "SM") and (d2 == "SM" or d2 == "MS"):
        return 1
    else:
        return 0


def load(file: str) -> list[str]:
    with open(file, "r") as fid:
        data = fid.readlines()
    return [d.strip() for d in data]


def main(array) -> int:
    final = 0
    for r, c in product(range(1,len(array)-1), range(1,len(array[0])-1)):
        if array[r][c] == "A":
            final += window(array, r, c)
    return final


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="A tool to solve aoc 2024 challenge")

    parser.add_argument("-i", "--input", help="input file", default="real.txt")
    args = parser.parse_args()
    data = load(args.input)
    res = main(data)
    print(res)
