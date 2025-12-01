import copy
from enum import Enum, auto
import time


class Direction(Enum):
    UP = auto()
    DOWN = auto()
    LEFT = auto()
    RIGHT = auto()


class Option(Enum):
    GUARD = auto()
    OBSTACLE = auto()
    VISITED = auto()
    NONE = auto()

class Field:
    x: int
    y: int
    values: list[list[Option]]
    direction: list[list[list[Direction]]]

    def __init__(self, lines=[""]):
        self.x = len(lines[0])
        self.y = len(lines)
        
        self.values = []
        self.direction = []
        for y in range(self.y):
            self.values.append([])
            self.direction.append([])
            for x in range(self.x):
                self.values[-1].append(Option.NONE)
                self.direction[-1].append([])


    def __copy__(self):
        input = ["a"*self.x] * self.y
        fc = Field(input)

        for row in self.values:
            fc.values = copy.copy(self.values)

        return fc

    def add(self, x: int, y: int, opt: Option, d: Direction = Direction.UP):
        self._set(x, y,opt)
        if opt == Option.GUARD:
            self.direction[y][x].append(d)

    def inside(self, x: int, y: int) -> bool:
        return x < self.x and x >= 0 and y < self.y and y >= 0

    def get(self, x: int, y: int) -> Option:
        assert self.inside(x, y), "can only add within bounds"
        return self.values[y][x]
    
    def _set(self, x: int, y: int, opt: Option) -> None:
        assert self.inside(x, y), "can only set a value in bounds"
        assert self.get(x, y) != Option.OBSTACLE, "can only modify locations that are not obstacles"
        self.values[y][x] = opt

    def num_visited(self) -> int:
        total = 0
        for rows in self.values:
            for type in rows:
                if type == Option.VISITED:
                    total += 1
        return total

    def __str__(self) -> str:
        out = "*** Field ***"
        for row in self.values:
            out += "\n"
            for type in row:
                match type:
                    case Option.NONE:
                        out += "."
                    case Option.GUARD:
                        out += "🚓"
                    case Option.OBSTACLE:
                        out += "#"
                    case Option.VISITED:
                        out += "X"
        return out


class Guard:
    x: int
    y: int
    direction: Direction

    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y
        self.direction = Direction.UP

    def __copy__(self):
        return Guard(self.x, self.y)

    def step(self):
        match self.direction:
            case Direction.UP:
                self.y -= 1
            case Direction.DOWN:
                self.y += 1
            case Direction.LEFT:
                self.x -= 1
            case Direction.RIGHT:
                self.x += 1

    def turn(self):
        match self.direction:
            case Direction.UP:
                self.direction = Direction.RIGHT
            case Direction.DOWN:
                self.direction = Direction.LEFT
            case Direction.LEFT:
                self.direction = Direction.UP
            case Direction.RIGHT:
                self.direction = Direction.DOWN


def solve(g: Guard, f: Field, show: bool) -> bool:
    """Given a field and a guard to solve the path of the guard.
    Returns true if the guard escapes mutates the field and the guard"""
    while True:
        new_guard = copy.copy(g)
        new_guard.step()
        if not f.inside(new_guard.x, new_guard.y):
            f.add(g.x, g.y, Option.VISITED)
            if show:
                print(f)
            break

        match f.get(new_guard.x, new_guard.y):
            case Option.OBSTACLE:
                g.turn()
                continue

            case Option.VISITED:
                prev_dirs = f.direction[new_guard.y][new_guard.x]
                if g.direction in prev_dirs:
                    return False

            case Option.GUARD:
                raise ValueError("Should not be reachable")

            case Option.NONE:
                pass
        
        f.add(g.x, g.y, Option.VISITED, g.direction)
        f.add(new_guard.x, new_guard.y, Option.GUARD)
        g = new_guard
        if show:
            print(f)

    return True


def main(g: Guard, f: Field, show: bool) -> int:
    first_field = copy.copy(f)
    _ = solve(copy.copy(g), first_field, show)

    block_locations = []
    for location, type in first_field.values.items():
        if type == Option.VISITED and location != g.location:
            block_locations.append(copy.copy(location))

    is_loop = 0
    for new_obstacle in block_locations:
        new_f = copy.copy(f)
        new_f.add(copy.copy(new_obstacle), Option.OBSTACLE)
        if not solve(copy.copy(g), new_f, False):
            is_loop += 1
    return is_loop  # last guard location before leaving


def load(file: str):
    with open(file, "r") as fid:
        data = fid.readlines()
        data = [d.strip() for d in data]
        g = Guard(-1, -1)
        f = Field(data)
        for y, line in enumerate(data):
            for x, val in enumerate(line):
                match val:
                    case "#":
                        f.add(x, y, Option.OBSTACLE, Direction.UP)
                    case "^":
                        f.add(x, y, Option.GUARD, g.direction)
                        g.x = x
                        g.y = y
    return (g, f)


def run(file, display=False):
    t0 = time.time()
    g, f = load(file)
    res = main(g, f, display)
    delta = time.time() - t0
    print(f"result= {res} duration = {delta}")
