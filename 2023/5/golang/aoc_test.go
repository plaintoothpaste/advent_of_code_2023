package main

import (
	"testing"
)

func TestAtoB(t *testing.T) {
	item := new("a", "b")
	item = item.add(50, 98, 2)
	item = item.add(52, 50, 48)
	test1 := item.aToB(98) == 50 && item.aToB(99) == 51
	test2 := item.aToB(53) == 55 && item.aToB(10) == 10

	if !test1 || !test2 {
		t.Fail()
	}
}

func TestExample(t *testing.T) {
	res := example()
	if res != 35 {
		t.Fail()
	}
}
func TestReal(t *testing.T) {
	if real() != 650599855 {
		t.Fail()
	}
}
