package main

import (
	"aoc/parse"
	"slices"
	"strings"
	"sync"
)

type converter struct {
	a        string
	b        string
	a_starts []uint
	ranges   []uint
	b_starts []uint
}

func new(a string, b string) converter {
	return converter{
		a: a,
		b: b,
	}
}

// inplace add new conversion range
func (m converter) add(b_start uint, a_start uint, r uint) converter {
	m.a_starts = append(m.a_starts, a_start)
	m.b_starts = append(m.b_starts, b_start)
	m.ranges = append(m.ranges, r)
	return m
}

func (m converter) aToB(in uint) uint {
	for i, a_lower := range m.a_starts {
		a_upper := a_lower + m.ranges[i]
		if in >= a_lower && in < a_upper {
			delta := in - a_lower
			return m.b_starts[i] + delta
		}
	}
	return in
}

func resolve(converters []converter, value uint) uint {
	for _, c := range converters {
		value = c.aToB(value)
	}
	return value
}

func run(file string) uint {
	seeds, data := parse.Run(file)
	// construct the converters
	converters := []converter{}
	for _, group := range data {
		lhs, rhs, _ := strings.Cut(group[0], "-to-")
		rhs = strings.TrimRight(rhs, " map:")
		c := new(lhs, rhs)
		for i := 1; i < len(group); i++ {
			single, _ := parse.LineToList[uint](group[i])
			c = c.add(single[0], single[1], single[2])
		}
		converters = append(converters, c)
	}

	var wg sync.WaitGroup
	wg.Add(len(seeds))
	results := make([]uint, len(seeds))
	for i, s := range seeds {
		go func(i int, s uint) {
			defer wg.Done()
			results[i] = resolve(converters, s)
		}(i, s)
	}
	wg.Wait()

	return slices.Min(results)
}

func example() uint {
	return run("../example.txt")
}
func real() uint {
	return run("../input.txt")
}
