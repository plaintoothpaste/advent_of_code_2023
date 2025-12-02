package parse

import (
	"reflect"
	"testing"
)

func TestToArray(t *testing.T) {
	input := "-25 +30 25"
	expected := []int{-25, 30, 25}
	actual, ok := LineToList[int](input)
	if !ok || !reflect.DeepEqual(expected, actual) {
		t.Fail()
	}
}
