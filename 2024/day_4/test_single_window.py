import pytest
from day_4 import window

testdata = [
    (["XMAS", "SAMX", "AAAA", "AAAA"], 2),
    (["XAAS", "MAAA", "AAAM", "SAAX"], 2),
    (["XAAX", "AMMA", "AAAA", "SAAS"], 2),
    (["SAAS", "AAAA", "AMMA", "XAAX"], 2),
    (["XMAS", "SAMX", "XMAS", "SAMX"], 4),
    (["XMAS", "MAMX", "AMAS", "SAMX"], 3),
    (["XMAS", "MAMX", "AMAS", "SAMX"], 3),
    (["XMAS", "MMMX", "AAAS", "SAMS"], 3),
]


@pytest.mark.parametrize("array,expected", testdata)
def test_all(array, expected):
    assert window(array, [0, 0]) == expected


testdata = [
    (["CCCCC", "CXMAS", "CSAMX", "CXMAS", "CSAMX"], 4),
    (["CCCCC", "CXMAS", "CMAMX", "CAMAS", "CSAMX"], 3),
    (["CCCCC", "CXMAS", "CMAMX", "CAMAS", "CSAMX"], 3),
    (["CCCCC", "CXMAS", "CMMMX", "CAAAS", "CSAMS"], 3),
]


@pytest.mark.parametrize("array,expected", testdata)
def test_offset(array, expected):
    assert window(array, [1, 1]) == expected


testdata = [
    (["XMAS", "SAMX", "AAAA", "AAAA", "CCCC"], 1),
    (["XAAS", "MAAA", "AAAM", "SAAX", "CCCC"], 1),
    (["XAAX", "AMMA", "AAAA", "SAAS", "CCCC"], 2),
    (["SAAS", "AAAA", "AMMA", "XAAX", "CCCC"], 2),
]


@pytest.mark.parametrize("array,expected", testdata)
def test_partial(array, expected):
    assert window(array, [0, 0]) == expected
