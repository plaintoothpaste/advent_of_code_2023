import copy
from functools import reduce


def main(raw_lines: list[str]) -> int:
    sum = 0
    for line in raw_lines:
        int_line = split(line)
        assert len(int_line) > 1
        if check_descend(int_line) or check_ascend(int_line):
            sum += 1

        
    return sum


def split(line) -> list[int]:
    return [int(d) for d in line.split(" ")]


def reduce_check(previous: int, variable: int):
    if previous <= -1:
        return previous

    delta = previous - variable
    if delta > 3 or delta < 1:
        return -1
    else:
        return variable


def check_descend(line: list[int]) -> bool:
    delta = reduce(reduce_check, line)
    if delta != -1:
        return True
    else:
        for i in range(len(line)):
            copier = copy.copy(line)
            copier.pop(i)
            delta = reduce(reduce_check, copier)
            if delta != -1:
                return True
    return False


def check_ascend(line: list[int]) -> bool:
    line.reverse()
    return check_descend(line)
