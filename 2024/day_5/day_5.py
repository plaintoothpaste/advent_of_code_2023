import argparse
import math


def mid_page(refs: dict[int, list[int]], manual: list[int]) -> int:
    for i, page in enumerate(manual):
        if page not in refs:
            continue

        if any(n in manual[0:i] for n in refs[page]):
            return 0
    mid = math.floor(len(manual) / 2)
    return manual[mid]


def main(refs: dict[int, list[int]], manuals: list[list[int]]) -> int:
    return sum(mid_page(refs, man) for man in manuals)


def load(file: str) -> tuple[dict[int, list[int]], list[list[int]]]:
    with open(file, "r") as fid:
        data = fid.readlines()

    refs = {}
    manuals = []
    for line in data:
        if "|" in line:
            vals = [int(i) for i in line.split("|")]
            if vals[0] in refs:
                refs[vals[0]].append(vals[1])
            else:
                refs[vals[0]] = [vals[1]]

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
