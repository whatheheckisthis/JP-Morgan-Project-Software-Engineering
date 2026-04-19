import json
from pathlib import Path


def test_fixture_vectors_are_versioned_json() -> None:
    fixture = Path("eicvs/tests/fixtures/sql_injection_vectors.json")
    payload = json.loads(fixture.read_text(encoding="utf-8"))
    assert payload["class"] == "sql_injection"
    assert payload["positive"]
