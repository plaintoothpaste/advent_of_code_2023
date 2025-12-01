import argparse


def removeDoDont(data: str) -> str:
    starts = 0
    ends = 0
    out = ""
    while ends !=-1:
        starts = data.find("do()",starts, len(data))
        ends = data.find("don't()",starts, len(data))
        out += data[starts+4:ends]
        starts = ends + 7

    return out



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
        text = "do()"+fid.read()

    text = removeDoDont(text)
    return sum(map(findSum, text.split("mul(")))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="A tool to solve aoc 2024 challenge")

    parser.add_argument("-i", "--input", help="input file", default="real.txt")
    args = parser.parse_args()
    res = main(args.input)
    print(res)