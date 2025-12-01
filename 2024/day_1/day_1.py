import argparse


def main(lhs: list[int], rhs: list[int]) -> None:
    lhs.sort()
    rhs.sort()

    delta = [abs(a-b) for a, b in zip(lhs, rhs)]
    print(sum(delta))


def setup(file: str) -> tuple[list[int]]:
    with open(file, "r") as fid:
        lines = fid.readlines()
    lhs = []
    rhs = []
    for line in lines:
        items = line.split()
        lhs.append(int(items[0]))
        rhs.append(int(items[-1]))
    return (lhs, rhs)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="A tool to solve aoc 2024 challenge"
    )

    parser.add_argument("-i", "--input", help="input file",
                        default="example.txt")
    args = parser.parse_args()
    lhs, rhs = setup(args.input)
    main(lhs, rhs)
