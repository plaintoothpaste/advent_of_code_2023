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


class Coord:
    x: int
    y: int

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, value):
        return self.x == value.x and self.y == value.y

    def __hash__(self):
        # enables:  a in list[self]
        return hash((self.x, self.y))


class Field:
    x: int
    y: int
    values: dict[Coord, Option]

    def __init__(self, lines):
        self.x = len(lines[0])
        self.y = len(lines)
        self.values = {}

    def add(self, x: int, y: int, opt: Option):
        assert self.inside(x, y), "can only add within bounds"

        c = Coord(x, y)
        if c in self.values:
            option = self.values[c]
            assert option != Option.OBSTACLE, "Opstacles can not be changed onced added"
            self.values[c] = opt
        else:
            self.values[c] = opt

    def inside(self, x: int, y: int) -> bool:
        return x < self.x and x >= 0 and y < self.y and y >= 0

    def exist(self, x: int, y: int) -> Option:
        assert self.inside(x, y), "can only add within bounds"
        c = Coord(x, y)
        return self.values.get(c, Option.NONE)

    def num_visited(self) -> int:
        total = 0
        for key, val in self.values.items():
            if val == Option.VISITED:
                total += 1
        return total

    def __str__(self) -> str:
        out = "*** Field ***"
        for yi in range(self.y):
            out += "\n"
            for xi in range(self.x):
                match self.exist(xi, yi):
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
    location: Coord
    direction: Direction

    def __init__(self, x, y):
        self.location = Coord(x, y)
        self.direction = Direction.UP

    def step(self):
        match self.direction:
            case Direction.UP:
                self.location.y -= 1
            case Direction.DOWN:
                self.location.y += 1
            case Direction.LEFT:
                self.location.x -= 1
            case Direction.RIGHT:
                self.location.x += 1

    def step_back(self):
        match self.direction:
            case Direction.UP:
                self.location.y += 1
            case Direction.DOWN:
                self.location.y -= 1
            case Direction.LEFT:
                self.location.x += 1
            case Direction.RIGHT:
                self.location.x -= 1

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


def main(g: Guard, f: Field, show: bool) -> int:
    while True:
        if show:
            print(f)
        ox = g.location.x
        oy = g.location.y
        g.step()
        if not f.inside(g.location.x, g.location.y):
            break

        new_location = f.exist(g.location.x, g.location.y)
        match new_location:
            case Option.OBSTACLE:
                g.step_back()
                g.turn()
            case Option.NONE | Option.VISITED:
                f.add(ox, oy, Option.VISITED)
                f.add(g.location.x, g.location.y, Option.GUARD)

    return f.num_visited() + 1  # last guard location before leaving


def load(file: str):
    with open(file, "r") as fid:
        data = fid.readlines()
        g = Guard(-1, -1)
        f = Field(data)
        for y, line in enumerate(data):
            for x, val in enumerate(line):
                match val:
                    case "#":
                        f.add(x, y, Option.OBSTACLE)
                    case "^":
                        f.add(x, y, Option.GUARD)
                        g.location = Coord(x, y)
    return (g, f)


def run(file, display=False):
    t0 = time.time()
    g, f = load(file)
    res = main(g, f, display)
    delta = time.time() - t0
    print(f"result= {res} duration = {delta}")
