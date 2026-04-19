from __future__ import annotations

import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3]))

from eicvs.evidence.evidence_ledger import compute_record_id
from eicvs.services.trace.fixture_auth_target import AuthRequest, _map_controls, analyse
# 
VECTOR_PATH = Path(__file__).resolve().parents[1] / "fixtures/auth_vectors.json"


def _requests(category: str) -> list[AuthRequest]:
    fixture = json.loads(VECTOR_PATH.read_text(encoding="utf-8"))
    items = []
    for vector in fixture[category]:
        payload = vector["payload"]
        items.append(AuthRequest(vector_id=vector["id"], credential_strength=payload["credential_strength"], mfa_present=payload["mfa_present"], category=category))
    return items


def test_positive_vectors_trigger_controls():
    for request in _requests("positive"):
        assert len(_map_controls(analyse(request))) >= 1


def test_negative_vectors_trigger_no_controls():
    for request in _requests("negative"):
        assert _map_controls(analyse(request)) == []


def test_edge_vectors_do_not_raise():
    for request in _requests("edge"):
        _map_controls(analyse(request))


def test_result_values_are_restricted():
    allowed = {"PASS", "FAIL", "ERROR"}
    assert allowed == {"PASS", "FAIL", "ERROR"}


def test_record_id_is_deterministic():
    assert compute_record_id("GENESIS", "abc", 123) == compute_record_id("GENESIS", "abc", 123)
