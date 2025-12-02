package parse

import (
	"os"
	"strconv"
	"strings"
)

func load(file string) []string {
	data, err := os.ReadFile(file)
	if err != nil {
		panic(err)
	}
	split := strings.Split(string(data), "\n")
	var out []string
	for _, line := range split {
		l := strings.TrimSpace(line)
		if len(l) != 0 {
			out = append(out, l)
		}
	}
	return out
}

func LineToList[T int | uint](line string) ([]T, bool) {
	out := []T{}
	if !strings.ContainsAny(strings.Split(line, "")[0], "0123456789-+") {
		return out, false
	}
	converter := func(s string) T {
		val, err := strconv.Atoi(s)
		if err != nil {
			panic("conversion failed")
		}
		return T(val)
	}
	//https://boldlygo.tech/archive/2024-05-27-type-switches-with-generics/
	switch any(T(0)).(type) {
	case int:
		// already above
	case uint:
		converter = func(s string) T {
			val, err := strconv.Atoi(s)
			if err != nil || val < 0 {
				panic("conversion failed")
			}
			return T(val)
		}
	}

	for _, part := range strings.Split(line, " ") {
		out = append(out, converter(part))
	}
	return out, true
}

// challenge specific implementation
func Run(file string) ([]uint, [][]string) {
	lines := load(file)
	lhs, _ := LineToList[uint](strings.TrimLeft(lines[0], "seeds: "))

	var grouper [][]string
	subgroup := []string{}
	for i, line := range lines[1:] {
		if strings.Contains(line, "map:") && i > 0 {
			grouper = append(grouper, subgroup)
			subgroup = []string{
				line,
			}
		} else {
			subgroup = append(subgroup, line)
		}
	}
	grouper = append(grouper, subgroup)

	return lhs, grouper
}
