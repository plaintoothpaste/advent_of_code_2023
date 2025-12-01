import argparse
import math


def is_valid(refs: list[tuple[int, int]], manual: list[int]) -> bool:
    for i, page in enumerate(manual):
        for lhs, rhs in refs:
            if page != lhs:
                continue

            if rhs in manual[0:i]:
                return False
    return True


def mid_page(refs: list[tuple[int, int]], manual: list[int]) -> int:
    """only bad manuals"""
    for i, page in enumerate(manual):
        for lhs, rhs in refs:
            if page != lhs:
                continue

            if rhs in manual[0:i]:
                x = manual.index(rhs, 0, i)
                manual[x] = page
                manual[i] = rhs
                return mid_page(refs, manual)

    mid = math.floor(len(manual) / 2)
    return manual[mid]


def main(refs: list[tuple[int, int]], manuals: list[list[int]]) -> int:
    return sum(mid_page(refs, man) for man in manuals if not is_valid(refs, man))


def load(file: str) -> tuple[list[tuple[int, int]], list[list[int]]]:
    with open(file, "r") as fid:
        data = fid.readlines()

    refs = []
    manuals = []
    for line in data:
        if "|" in line:
            refs.append(tuple(int(i) for i in line.split("|")))

        if "," in line:
            manuals.append([int(i) for i in line.split(",")])

    return (refs, manuals)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="A tool to solve aoc 2024 challenge")

    parser.add_argument("-i", "--input", help="input file", default="example.txt")
    args = parser.parse_args()
    refs, manuals = load(args.input)
    res = main(refs, manuals)
    print(res)
