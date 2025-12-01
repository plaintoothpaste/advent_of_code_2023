import argparse
from typing import Iterator


def main(file: str) -> None:
    store: dict[int, list[int,int]] = {}
    for lhs, rhs in yielder(file):
        if lhs not in store:
            store[lhs] = [0,0]
        if rhs not in store:
            store[rhs] = [0,0]
        
        store[lhs][0] += 1
        store[rhs][1] += 1

    total = 0
    for key, occurance in store.items():
        total += key * occurance[0] * occurance[1]
    print(total)


def yielder(file: str) -> Iterator[tuple[int, int]]:
    with open(file, "r") as fid:
        for line in fid.readlines():
            items = line.split()
            yield (int(items[0]), int(items[-1]))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="A tool to solve aoc 2024 challenge"
    )

    parser.add_argument("-i", "--input", help="input file",
                        default="real.txt")
    args = parser.parse_args()
    main(args.input)
