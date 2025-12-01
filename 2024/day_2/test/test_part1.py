import day_2 as runner
import pytest
from pathlib import Path


def yield_text():
    files = [
        "example.txt",
        "real.txt",
    ]
    for f in files:
        yield Path(f).read_text().splitlines()


@pytest.mark.parametrize("data", yield_text())
def test_both(data: list[str]):
    result = runner.main(data)
    print(result)
    assert result > 0
