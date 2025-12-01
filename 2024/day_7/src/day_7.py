from pathlib import Path

def splitter(input: str) -> tuple[int, list[int]]:
    data = input.split(":")
    second_part = data[1].strip().split(" ")
    vals = [int(d) for d in second_part]
    return (int(data[0]), vals)


def solver(res: int, input: list[int], total: int = 0) -> int:
    """Returns the res or 0 if not solvable"""
    if total == res:
        return total
    if total > res:
        return 0

    if len(input) == 0:
        return 0

    new_list = input.copy()
    v = new_list.pop(0)
    summer_result = solver(res, new_list, total + v)
    mult_result = solver(res, new_list.copy(), total * v)

    return summer_result if summer_result > mult_result else mult_result

def run(file: str) -> int:
    total = 0
    for line in Path(file).read_text().splitlines():
        test_val, parameters = splitter(line)
        starting = parameters.pop(0)
        total += solver(test_val, parameters, starting)
    
    return total