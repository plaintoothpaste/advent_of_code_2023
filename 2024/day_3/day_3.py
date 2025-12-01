import argparse


def findSum(part: str) -> int:
    if len(part) < 3:
        return 0

    numbers = part.split(")")[0]
    if any(n not in "1234567890," for n in numbers):
        return 0

    singles = [int(n) for n in numbers.split(",")]
    if len(singles) != 2:
        return 0
    return singles[0] * singles[1]


def main(file: str) -> int:
    with open(file, "r") as fid:
        text = fid.read()
    sum(map(findSum, text.split("mul(")))
    return sum(map(findSum, text.split("mul(")))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="A tool to solve aoc 2024 challenge")

    parser.add_argument("-i", "--input", help="input file", default="real.txt")
    args = parser.parse_args()
    res = main(args.input)
    print(res)
