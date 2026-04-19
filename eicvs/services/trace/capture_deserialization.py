from __future__ import annotations

import csv
import hashlib
import json
import subprocess
import time
from pathlib import Path
from typing import Final

from eicvs.evidence.evidence_ledger import (
    ASVS_VERSION,
    GAP_REGISTER_PATH,
    LEDGER_PATH,
    LOOPBACK_HOST,
    OWASP_TOP10_VERSION,
    SCHEMA_VERSION,
    EvidenceRecord,
    append_record,
    compute_record_id,
)
from eicvs.services.trace.fixture_deserialization_target import DeserializationRequest, _map_controls, analyse

CLASS_ID: Final[str] = "deserialization"


def _fixture_to_requests(fixture: dict) -> list[DeserializationRequest]:
    requests: list[DeserializationRequest] = []
    for category in ("positive", "negative", "edge"):
        for vector in fixture[category]:
            payload = vector["payload"]
            requests.append(DeserializationRequest(vector_id=vector["id"], source_trusted=payload["source_trusted"], coercion_guard_enabled=payload["coercion_guard_enabled"], type_name=payload["type_name"], category=category))
    return requests


def _last_record_id() -> str:
    ledger = Path(__file__).resolve().parents[2] / LEDGER_PATH
    if not ledger.exists() or ledger.stat().st_size == 0:
        return "GENESIS"
    with ledger.open("r", encoding="utf-8", newline="") as handle:
        rows = list(csv.DictReader(handle))
    if not rows:
        return "GENESIS"
    return rows[-1]["record_id"]


def run(vector_path: str) -> EvidenceRecord:
    fixture_path = Path(vector_path)
    if not fixture_path.exists():
        raise FileNotFoundError(vector_path)

    fixture = json.loads(fixture_path.read_text(encoding="utf-8"))
    for key in ("positive", "negative", "edge"):
        if key not in fixture or not isinstance(fixture[key], list):
            raise ValueError("fixture arrays")

    requests = _fixture_to_requests(fixture)
    analyses = [analyse(request) for request in requests]

    controls = []
    for analysis in analyses:
        controls.extend(_map_controls(analysis))

    timestamp_ns = time.time_ns()
    reports_dir = Path(__file__).resolve().parents[2] / "reports"
    reports_dir.mkdir(parents=True, exist_ok=True)
    junit_path = reports_dir / f"{CLASS_ID}_{timestamp_ns}.xml"
    try:
        test_path = Path(__file__).resolve().parents[2] / "tests" / "asvs" / "test_deserialization_controls.py"
        pytest_rc = subprocess.run([
            "pytest",
            "-q",
            str(test_path),
            f"--junitxml={str(junit_path)}",
        ], check=False, capture_output=True, text=True)
        if not junit_path.exists():
            result = "ERROR"
        elif pytest_rc.returncode == 0:
            result = "PASS"
        else:
            result = "FAIL"
    except OSError as exc:
        raise RuntimeError("pytest invocation failed") from exc

    input_fp = hashlib.sha256(fixture_path.read_bytes()).hexdigest()
    previous_record_id = _last_record_id()
    record_id = compute_record_id(previous_record_id, input_fp, timestamp_ns)
    record = EvidenceRecord(
        record_id=record_id,
        class_id=CLASS_ID,
        input_fp=input_fp,
        timestamp_ns=timestamp_ns,
        control_triggers=controls,
        result=result,
        previous_record_id=previous_record_id,
    )
    append_record(record)
    return record
