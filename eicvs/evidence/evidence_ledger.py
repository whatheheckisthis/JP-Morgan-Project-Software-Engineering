from __future__ import annotations

import csv
import hashlib
import json
import logging
from dataclasses import dataclass
from pathlib import Path
from typing import Final

SCHEMA_VERSION: Final[str] = "1.0.0"
ASVS_VERSION: Final[str] = "4.0.3"
OWASP_TOP10_VERSION: Final[str] = "2021"
LOOPBACK_HOST: Final[str] = "127.0.0.1"
LEDGER_PATH: Final[str] = "evidence/evidence_ledger.csv"
GAP_REGISTER_PATH: Final[str] = "evidence/evidence_gap_register.csv"

LOGGER = logging.getLogger(__name__)
ROOT_DIR = Path(__file__).resolve().parents[1]
_VALID_RESULTS: Final[set[str]] = {"PASS", "FAIL", "ERROR"}


@dataclass(frozen=True)
class ControlTrigger:
    asvs_id: str
    owasp_ref: str


@dataclass(frozen=True)
class EvidenceRecord:
    record_id: str
    class_id: str
    input_fp: str
    timestamp_ns: int
    control_triggers: list[ControlTrigger]
    result: str
    previous_record_id: str


def compute_record_id(previous_record_id: str, input_fp: str, timestamp_ns: int) -> str:
    payload = f"{previous_record_id}{input_fp}{timestamp_ns}".encode("utf-8")
    return hashlib.sha256(payload).hexdigest()


def append_record(record: EvidenceRecord) -> None:
    if record.result not in _VALID_RESULTS:
        raise ValueError("result")
    ledger = ROOT_DIR / LEDGER_PATH
    ledger.parent.mkdir(parents=True, exist_ok=True)
    file_exists = ledger.exists()
    with ledger.open("a", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle)
        if not file_exists:
            writer.writerow([
                "record_id",
                "class_id",
                "input_fp",
                "timestamp_ns",
                "control_triggers",
                "result",
                "previous_record_id",
            ])
        writer.writerow([
            record.record_id,
            record.class_id,
            record.input_fp,
            record.timestamp_ns,
            json.dumps([trigger.__dict__ for trigger in record.control_triggers], separators=(",", ":")),
            record.result,
            record.previous_record_id,
        ])


def verify_chain() -> bool:
    ledger = ROOT_DIR / LEDGER_PATH
    if not ledger.exists():
        raise FileNotFoundError(LEDGER_PATH)
    with ledger.open("r", encoding="utf-8", newline="") as handle:
        reader = csv.DictReader(handle)
        for index, row in enumerate(reader):
            if index == 0 and row["previous_record_id"] != "GENESIS":
                LOGGER.error("Inconsistent record at index=%s record_id=%s", index, row["record_id"])
                return False
            expected = compute_record_id(
                previous_record_id=row["previous_record_id"],
                input_fp=row["input_fp"],
                timestamp_ns=int(row["timestamp_ns"]),
            )
            if expected != row["record_id"]:
                LOGGER.error("Inconsistent record at index=%s record_id=%s", index, row["record_id"])
                return False
    return True
